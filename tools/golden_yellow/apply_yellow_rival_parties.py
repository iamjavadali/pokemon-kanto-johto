#!/usr/bin/env python3
"""Overlay canonical Pokemon Yellow rival rosters onto the FRLG trainer source.

The base trainer file remains the upstream-compatible source for trainer metadata and
numeric ordering.  This filter replaces only the Pokemon roster portion of the rival
records used by Stage 1 Golden Yellow, preserving names, classes, pictures, music,
AI flags, and the original per-record IV profile.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

IV_DEFAULT = "IVs: 0 HP / 0 Atk / 0 Def / 0 SpA / 0 SpD / 0 Spe"


def team(*mons: tuple[str, int]) -> tuple[tuple[str, int], ...]:
    return mons


OAK = team(("Eevee", 5))
ROUTE22_EARLY = team(("Spearow", 9), ("Eevee", 8))
CERULEAN = team(("Spearow", 18), ("Sandshrew", 15), ("Rattata", 15), ("Eevee", 17))
SS_ANNE = team(("Spearow", 19), ("Rattata", 16), ("Sandshrew", 18), ("Eevee", 20))

TOWER_JOLTEON = team(("Fearow", 25), ("Shellder", 23), ("Vulpix", 22), ("Sandshrew", 20), ("Eevee", 25))
TOWER_FLAREON = team(("Fearow", 25), ("Magnemite", 23), ("Shellder", 22), ("Sandshrew", 20), ("Eevee", 25))
TOWER_VAPOREON = team(("Fearow", 25), ("Vulpix", 23), ("Magnemite", 22), ("Sandshrew", 20), ("Eevee", 25))

SILPH_JOLTEON = team(("Sandslash", 38), ("Ninetales", 35), ("Cloyster", 37), ("Kadabra", 35), ("Jolteon", 40))
SILPH_FLAREON = team(("Sandslash", 38), ("Cloyster", 35), ("Magneton", 37), ("Kadabra", 35), ("Flareon", 40))
SILPH_VAPOREON = team(("Sandslash", 38), ("Magneton", 35), ("Ninetales", 37), ("Kadabra", 35), ("Vaporeon", 40))

ROUTE22_LATE_JOLTEON = team(("Sandslash", 47), ("Exeggcute", 45), ("Ninetales", 45), ("Cloyster", 47), ("Kadabra", 50), ("Jolteon", 53))
ROUTE22_LATE_FLAREON = team(("Sandslash", 47), ("Exeggcute", 45), ("Cloyster", 45), ("Magneton", 47), ("Kadabra", 50), ("Flareon", 53))
ROUTE22_LATE_VAPOREON = team(("Sandslash", 47), ("Exeggcute", 45), ("Magneton", 45), ("Ninetales", 47), ("Kadabra", 50), ("Vaporeon", 53))

CHAMPION_JOLTEON = team(("Sandslash", 61), ("Alakazam", 59), ("Exeggutor", 61), ("Cloyster", 61), ("Ninetales", 63), ("Jolteon", 65))
CHAMPION_FLAREON = team(("Sandslash", 61), ("Alakazam", 59), ("Exeggutor", 61), ("Magneton", 61), ("Cloyster", 63), ("Flareon", 65))
CHAMPION_VAPOREON = team(("Sandslash", 61), ("Alakazam", 59), ("Exeggutor", 61), ("Ninetales", 61), ("Magneton", 63), ("Vaporeon", 65))

# Existing numeric slots are deliberately retained.  For three-way FRLG starter
# groups: Squirtle slot = Jolteon lane, Bulbasaur = Flareon, Charmander = Vaporeon.
OVERRIDES = {
    "TRAINER_RIVAL_OAKS_LAB_SQUIRTLE": OAK,
    "TRAINER_RIVAL_OAKS_LAB_BULBASAUR": OAK,
    "TRAINER_RIVAL_OAKS_LAB_CHARMANDER": OAK,
    "TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE": ROUTE22_EARLY,
    "TRAINER_RIVAL_ROUTE22_EARLY_BULBASAUR": ROUTE22_EARLY,
    "TRAINER_RIVAL_ROUTE22_EARLY_CHARMANDER": ROUTE22_EARLY,
    "TRAINER_RIVAL_CERULEAN_SQUIRTLE": CERULEAN,
    "TRAINER_RIVAL_CERULEAN_BULBASAUR": CERULEAN,
    "TRAINER_RIVAL_CERULEAN_CHARMANDER": CERULEAN,
    "TRAINER_RIVAL_SS_ANNE_SQUIRTLE": SS_ANNE,
    "TRAINER_RIVAL_SS_ANNE_BULBASAUR": SS_ANNE,
    "TRAINER_RIVAL_SS_ANNE_CHARMANDER": SS_ANNE,
    "TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE": TOWER_JOLTEON,
    "TRAINER_RIVAL_POKEMON_TOWER_BULBASAUR": TOWER_FLAREON,
    "TRAINER_RIVAL_POKEMON_TOWER_CHARMANDER": TOWER_VAPOREON,
    "TRAINER_RIVAL_SILPH_SQUIRTLE": SILPH_JOLTEON,
    "TRAINER_RIVAL_SILPH_BULBASAUR": SILPH_FLAREON,
    "TRAINER_RIVAL_SILPH_CHARMANDER": SILPH_VAPOREON,
    "TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE": ROUTE22_LATE_JOLTEON,
    "TRAINER_RIVAL_ROUTE22_LATE_BULBASAUR": ROUTE22_LATE_FLAREON,
    "TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER": ROUTE22_LATE_VAPOREON,
    "TRAINER_CHAMPION_FIRST_SQUIRTLE": CHAMPION_JOLTEON,
    "TRAINER_CHAMPION_FIRST_BULBASAUR": CHAMPION_FLAREON,
    "TRAINER_CHAMPION_FIRST_CHARMANDER": CHAMPION_VAPOREON,
    # Prevent the FRLG postgame starter teams (including Gen II species) from
    # leaking into Stage 1 if the Champion room is revisited before later phases.
    "TRAINER_CHAMPION_REMATCH_SQUIRTLE": CHAMPION_JOLTEON,
    "TRAINER_CHAMPION_REMATCH_BULBASAUR": CHAMPION_FLAREON,
    "TRAINER_CHAMPION_REMATCH_CHARMANDER": CHAMPION_VAPOREON,
}

BLOCK_RE = re.compile(r"(?ms)^=== (TRAINER_[A-Z0-9_]+) ===\n(.*?)(?=^=== |\Z)")

JESSIE_JAMES_MT_MOON = """=== TRAINER_JESSIE_JAMES_MT_MOON ===
Name: JESSIE&J.
Class: Rocket Grunt Frlg
Pic: Jessie And James
Gender: Male
Music: Rocket
Double Battle: No
AI: Check Bad Move

Ekans
Level: 14
IVs: 0 HP / 0 Atk / 0 Def / 0 SpA / 0 SpD / 0 Spe

Meowth
Level: 14
IVs: 0 HP / 0 Atk / 0 Def / 0 SpA / 0 SpD / 0 Spe

Koffing
Level: 14
IVs: 0 HP / 0 Atk / 0 Def / 0 SpA / 0 SpD / 0 Spe
"""


def replace_block(match: re.Match[str], seen: set[str]) -> str:
    name = match.group(1)
    body = match.group(2)
    roster = OVERRIDES.get(name)
    if roster is None:
        return match.group(0)

    lines = body.rstrip("\n").splitlines()
    first_mon = None
    for i in range(len(lines) - 1):
        if lines[i + 1].startswith("Level:"):
            first_mon = i
            break
    if first_mon is None:
        raise SystemExit(f"Could not locate Pokemon roster in {name}")

    iv_line = next((line for line in lines[first_mon:] if line.startswith("IVs:")), IV_DEFAULT)
    metadata = lines[:first_mon]
    while metadata and metadata[-1] == "":
        metadata.pop()

    new_lines = list(metadata)
    for species, level in roster:
        new_lines.extend(["", species, f"Level: {level}", iv_line])

    seen.add(name)
    return f"=== {name} ===\n" + "\n".join(new_lines) + "\n\n"


def main() -> int:
    if len(sys.argv) != 2:
        raise SystemExit("usage: apply_yellow_rival_parties.py <trainers_frlg.party>")

    source = Path(sys.argv[1]).read_text(encoding="utf-8")
    seen: set[str] = set()
    output = BLOCK_RE.sub(lambda m: replace_block(m, seen), source)

    missing = sorted(set(OVERRIDES) - seen)
    if missing:
        raise SystemExit("Missing expected rival trainer records: " + ", ".join(missing))

    if "=== TRAINER_JESSIE_JAMES_MT_MOON ===" not in output:
        if not output.endswith("\n"):
            output += "\n"
        output += "\n" + JESSIE_JAMES_MT_MOON

    sys.stdout.write(output)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
