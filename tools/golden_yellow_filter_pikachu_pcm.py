#!/usr/bin/env python3
"""Prepare Pokemon Yellow 1-bit Pikachu cries for the GBA m4a mixer."""

import argparse
import math
import wave
from pathlib import Path

REQUIRED_IDS = (1, 2, 3, 4, 5, 6, 9, 10, 11, 13, 15, 17, 18, 19, 20, 25, 26, 28, 29, 31, 33, 34, 35, 37, 38, 39, 40)
SAMPLE_RATE = 22050
HIGH_PASS_HZ = 30.0
LOW_PASS_HZ = 4500.0
TARGET_PEAK_DBFS = -6.0
BUTTERWORTH_Q = (0.5411961, 1.306563)


def low_pass_coefficients(cutoff, sample_rate, q):
    omega = 2.0 * math.pi * cutoff / sample_rate
    cosine = math.cos(omega)
    sine = math.sin(omega)
    alpha = sine / (2.0 * q)
    a0 = 1.0 + alpha
    return (((1.0 - cosine) / 2.0) / a0, (1.0 - cosine) / a0,
            ((1.0 - cosine) / 2.0) / a0, (-2.0 * cosine) / a0,
            (1.0 - alpha) / a0)


def apply_biquad(samples, coefficients):
    b0, b1, b2, a1, a2 = coefficients
    output = []
    x1 = x2 = y1 = y2 = 0.0
    for sample in samples:
        value = b0 * sample + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2
        output.append(value)
        x2, x1 = x1, sample
        y2, y1 = y1, value
    return output


def filter_cry(source, destination):
    with wave.open(str(source), "rb") as wav:
        if wav.getnchannels() != 1 or wav.getsampwidth() != 1 or wav.getframerate() != SAMPLE_RATE:
            raise ValueError(f"{source}: expected mono 8-bit PCM at {SAMPLE_RATE} Hz")
        raw = wav.readframes(wav.getnframes())

    samples = [(value / 255.0) * 2.0 - 1.0 for value in raw]
    rc = 1.0 / (2.0 * math.pi * HIGH_PASS_HZ)
    alpha = rc / (rc + 1.0 / SAMPLE_RATE)
    high_passed = []
    previous_input = samples[0]
    previous_output = 0.0
    for sample in samples:
        previous_output = alpha * (previous_output + sample - previous_input)
        previous_input = sample
        high_passed.append(previous_output)

    filtered = high_passed
    for q in BUTTERWORTH_Q:
        filtered = apply_biquad(filtered, low_pass_coefficients(LOW_PASS_HZ, SAMPLE_RATE, q))

    mean = sum(filtered) / len(filtered)
    filtered = [sample - mean for sample in filtered]
    peak = max(abs(sample) for sample in filtered)
    target_peak = 10.0 ** (TARGET_PEAK_DBFS / 20.0)
    gain = target_peak / peak if peak else 1.0
    encoded = bytes(
        max(0, min(255, math.floor(((max(-1.0, min(1.0, sample * gain)) + 1.0) * 127.5) + 0.5)))
        for sample in filtered
    )

    destination.parent.mkdir(parents=True, exist_ok=True)
    with wave.open(str(destination), "wb") as wav:
        wav.setnchannels(1)
        wav.setsampwidth(1)
        wav.setframerate(SAMPLE_RATE)
        wav.writeframes(encoded)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source_dir", type=Path)
    parser.add_argument("output_dir", type=Path)
    args = parser.parse_args()
    for cry_id in REQUIRED_IDS:
        filter_cry(args.source_dir / f"pikachu_cry_{cry_id}.wav",
                   args.output_dir / f"yellow_pikachu_{cry_id:02d}.wav")


if __name__ == "__main__":
    main()
