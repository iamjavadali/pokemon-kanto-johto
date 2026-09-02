# Pokémon Golden Yellow — P9A Yellow Pikachu PCM Reachability Audit

**Document type:** Authoritative P9A source/call-site audit and implementation gate  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical branch:** `prototype/v0.1`  
**Status:** **P9A LOCKED — Option B Reachability Policy approved**  
**Baseline parent:** `298dd506bc510fc8f1e0a2e1c004a37609218ff1`

---

# 1. Purpose

This document locks the P9A Yellow Partner Pikachu PCM audit before any original Yellow PCM asset is imported or any runtime playback path is wired.

P9A answers, for every one of Pokémon Yellow's 42 canonical Pikachu PCM assets:

1. what Yellow call site(s) use the clip;
2. what current or future Golden Yellow reaction/event corresponds to those call sites;
3. whether the clip is **Required**, **Deferred**, or **Not Applicable** under the approved reachability policy;
4. the converted GBA DirectSound ROM cost under Golden Yellow's existing `wav2agb` pipeline.

This file is the authoritative gate for P9B and later Yellow-audio work. No later pass may silently import all 42 clips merely because the source library contains 42 entries.

---

# 2. Locked Policy — Option B Reachability

Golden Yellow MUST preserve stable semantic IDs for the complete Yellow PCM namespace:

`YELLOW_PIKACHU_CRY_01` through `YELLOW_PIKACHU_CRY_42`.

However, ROM inclusion is reachability-driven:

- **Required** — import when P9B audio implementation begins because at least one corresponding Golden Yellow behavior already exists or is part of the current Partner-system behavior set.
- **Deferred** — do not import until the corresponding story/system boundary becomes active, unless a later explicit decision moves it forward.
- **Not Applicable** — do not import into the ROM when the original Yellow asset has no reachable gameplay call site, or the only identified source path is explicitly unreferenced.

If one clip has both Required and Deferred uses, the asset is classified **Required** because its required use already justifies ROM inclusion.

The approved initial inclusion set is therefore **27 Required clips**, not all 42.

---

# 3. Canonical PCM Classification

## 3.1 Required now — 27 clips

`01, 02, 03, 04, 05, 06, 09, 10, 11, 13, 15, 17, 18, 19, 20, 25, 26, 28, 29, 31, 33, 34, 35, 37, 38, 39, 40`

## 3.2 Deferred-only — 1 clip

`23`

Current known deferred-only use: Celadon Mansion 1F high-happiness Partner reaction.

## 3.3 Not Applicable / no reachable runtime use — 14 clips

`07, 08, 12, 14, 16, 21, 22, 24, 27, 30, 32, 36, 41, 42`

`Cry16` is explicitly excluded because the identified Yellow title routine that uses it is marked unreferenced in the source.

---

# 4. Authoritative 42-Clip Table

| PCM | Original Yellow call site(s) / Golden Yellow mapping | Disposition | GBA `.bin` | Aligned ROM |
|---:|---|---|---:|---:|
| 01 | Emotion 7 energetic/happy -> current Partner mood reaction; Yellow title opening deferred | Required | 9,619 B | 9,620 B |
| 02 | Oak's Lab "Pikachu dislikes Poké Balls" -> existing Yellow Oak/Partner opening | Required | 7,144 B | 7,144 B |
| 03 | PikaPic 0/1/29 internal portrait cry -> current neutral portrait path | Required | 10,642 B | 10,644 B |
| 04 | Emotion 22 Tower fear; Partner battle faint; overworld poison faint | Required | 16,879 B | 16,880 B |
| 05 | Emotion 10 affection; Emotion 20 max affection; Emotion 29/30 Fan Club | Required | 17,176 B | 17,176 B |
| 06 | Emotion 9 strong displeasure -> current mood reaction | Required | 19,090 B | 19,092 B |
| 07 | No reachable Yellow gameplay call site found | Not Applicable | 14,437 B | 14,440 B |
| 08 | No reachable Yellow gameplay call site found | Not Applicable | 17,737 B | 17,740 B |
| 09 | Emotion 27 restored-Bill shock -> accepted Bill reaction | Required | 14,008 B | 14,008 B |
| 10 | Emotion 14 highly agitated -> current mood reaction | Required | 33,808 B | 33,808 B |
| 11 | Special Partner battle opening when awake; title -> main-menu transition deferred | Required | 7,540 B | 7,540 B |
| 12 | No reachable Yellow gameplay call site found | Not Applicable | 14,272 B | 14,272 B |
| 13 | Emotion 17 unhappy/sad -> current mood reaction | Required | 20,608 B | 20,608 B |
| 14 | No reachable Yellow gameplay call site found | Not Applicable | 25,987 B | 25,988 B |
| 15 | Emotion 28 sick/status -> current Partner status reaction | Required | 21,664 B | 21,664 B |
| 16 | Title fallback routine explicitly marked unreferenced | Not Applicable | 21,763 B | 21,764 B |
| 17 | Yellow status screen canonical Partner cry -> modern status-screen equivalent | Required | 8,860 B | 8,860 B |
| 18 | PikaPic 18 internal cry -> current capture-success/V-sign portrait | Required | 15,889 B | 15,892 B |
| 19 | Emotion 23 Bill confused; Emotion 31 Bill post-state | Required | 23,446 B | 23,448 B |
| 20 | PikaPic 21 internal cry -> current fishing reaction | Required | 33,808 B | 33,808 B |
| 21 | No reachable Yellow gameplay call site found | Not Applicable | 33,792 B | 33,792 B |
| 22 | No reachable Yellow gameplay call site found | Not Applicable | 26,548 B | 26,548 B |
| 23 | Celadon Mansion 1F high-happiness Partner reaction | Deferred | 15,691 B | 15,692 B |
| 24 | No reachable Yellow gameplay call site found | Not Applicable | 29,749 B | 29,752 B |
| 25 | PikaPic 12 internal cry -> current frowning/mood portrait | Required | 15,031 B | 15,032 B |
| 26 | Emotion 32 Bill intermediate state -> current Bill reaction catalog | Required | 14,371 B | 14,372 B |
| 27 | No reachable Yellow gameplay call site found | Not Applicable | 11,995 B | 11,996 B |
| 28 | Thunder Stone/evolution refusal; PC deposit deferred; Day Care deposit deferred; Surfing Pikachu normal-result deferred | Required | 19,321 B | 19,324 B |
| 29 | Emotion 4 playful/content -> current mood reaction | Required | 12,853 B | 12,856 B |
| 30 | No reachable Yellow gameplay call site found | Not Applicable | 4,966 B | 4,968 B |
| 31 | Emotion 5 displeased -> current mood reaction | Required | 7,936 B | 7,936 B |
| 32 | No reachable Yellow gameplay call site found | Not Applicable | 5,857 B | 5,860 B |
| 33 | Emotion 16 strong happiness; Emotion 19 strong affection | Required | 13,414 B | 13,416 B |
| 34 | Emotion 15 content; Surfing Pikachu high-score result deferred | Required | 22,225 B | 22,228 B |
| 35 | Emotion 2 happy; Emotion 25 electric power; PC withdraw deferred; Day Care withdraw deferred; Hall of Fame deferred | Required | 19,189 B | 19,192 B |
| 36 | No reachable Yellow gameplay call site found | Not Applicable | 29,452 B | 29,452 B |
| 37 | Emotion 11 sleeping; Emotion 26 Pewter/Jigglypuff; Partner battle opening while asleep | Required | 14,767 B | 14,768 B |
| 38 | PikaPic 6 internal cry -> current irritated portrait | Required | 13,117 B | 13,120 B |
| 39 | Emotion 8 happy grin -> current mood reaction | Required | 18,529 B | 18,532 B |
| 40 | Emotion 3 subdued/neutral; Bill's PC release-refusal deferred | Required | 15,922 B | 15,924 B |
| 41 | No reachable Yellow gameplay call site found | Not Applicable | 25,459 B | 25,460 B |
| 42 | No reachable Yellow gameplay call site found | Not Applicable | 22,720 B | 22,720 B |

---

# 5. Locked Yellow `$FF` / NO-PCM Semantics

Yellow's emotion and PikaPic command formats use `$FF` to mean that no specific PCM clip is requested.

Golden Yellow MUST NOT reinterpret source `$FF` as "play the normal Partner Pikachu cry."

The final P9 audio model is:

1. ordinary `SPECIES_PIKACHU` -> existing modern Pikachu cry behavior;
2. canonical `SPECIES_PIKACHU_STARTER` outside a Yellow-owned audio override -> existing modern Partner cry behavior may remain;
3. Yellow command containing an explicit PCM ID -> play that exact Yellow PCM asset when the asset is reachable/imported;
4. Yellow command containing `$FF` -> **NO PCM** for that command;
5. PikaPic explicit PCM -> play the exact clip at the portrait-internal timing point;
6. PikaPic `$FF` -> **NO PCM** at that portrait timing point;
7. Yellow system-specific call sites such as battle entry, faint, status screen, Oak Lab, PC, Day Care, Hall of Fame, title, Celadon Mansion, and Surfing Pikachu must be wired only when their corresponding Golden Yellow behavior is applicable.

The following Emotion programs intentionally have no outer PCM request and must not receive a synthetic default Yellow/modern cry from the reaction command itself:

- Emotion 1;
- Emotion 6;
- Emotion 12;
- Emotion 18;
- Emotion 21;
- Emotion 24.

Emotion 13 contains no PCM command at all.

Several no-outer-PCM emotions legitimately receive sound later from their portrait programs:

- Emotion 1 -> PikaPic 1 -> Cry03;
- Emotion 6 -> PikaPic 6 -> Cry38;
- Emotion 12 -> PikaPic 12 -> Cry25;
- Emotion 18 -> PikaPic 18 -> Cry18;
- Emotion 21 -> PikaPic 21 -> Cry20.

Thunder Stone refusal is separate: the item/evolution event plays Cry28, while Emotion 24's subsequent portrait command remains PCM-silent.

---

# 6. Locked Non-Portrait Call-Site Corrections

P9B and later passes MUST preserve the following source-specific distinctions rather than collapsing them into generic Partner cry events:

- Bill's PC deposit -> Cry28;
- Bill's PC withdraw -> Cry35;
- Bill's PC release refused -> Cry40;
- Day Care deposit -> Cry28;
- Day Care withdraw -> Cry35;
- Partner battle entry awake -> Cry11;
- Partner battle entry asleep -> Cry37;
- Partner battle faint -> Cry04;
- Partner overworld poison faint -> Cry04;
- canonical Partner status screen -> Cry17;
- Oak Lab Poké Ball dislike event -> Cry02;
- Hall of Fame canonical Partner -> Cry35;
- Celadon Mansion high-happiness reaction -> Cry23;
- Surfing Pikachu normal result -> Cry28;
- Surfing Pikachu high-score result -> Cry34;
- title opening -> Cry01;
- title -> main-menu transition -> Cry11.

---

# 7. GBA ROM-Cost Baseline

The authoritative conversion baseline is Golden Yellow's existing compressed DirectSound path:

`wav2agb -b -c -l 1 --no-pad`

The cost table assumes the current converter's 16-byte sample header, 64-source-sample DPCM blocks, omitted final DPCM padding under `--no-pad`, and normal DirectSound 4-byte symbol alignment.

| Inclusion policy | Assets | Converted data | 4-byte-aligned ROM footprint |
|---|---:|---:|---:|
| Required now | 27 | 446,856 B | **446,892 B / 436.42 KiB** |
| Deferred-only | 1 | 15,691 B | **15,692 B / 15.32 KiB** |
| Not Applicable | 14 | 284,734 B | **284,752 B / 278.08 KiB** |
| Full 42-clip library | 42 | 747,281 B | **747,336 B / 729.82 KiB / 0.713 MiB** |

The approved reachability policy therefore avoids approximately **278.08 KiB** of ROM use from clips that have no current reachable Yellow gameplay purpose, while preserving their stable semantic IDs for future auditability.

---

# 8. P9B Implementation Gate

P9A is an audit/policy milestone, not audio acceptance.

The next implementation pass, P9B, MAY begin only from this locked baseline and MUST:

1. preserve IDs 01-42 even when an asset is not compiled into the ROM;
2. initially import only the 27 Required clips listed in Section 3.1;
3. keep Cry23 out until its deferred call site becomes active or explicit approval moves it forward;
4. keep all 14 Not Applicable clips out of the ROM unless a new verified Yellow call site changes their classification;
5. implement a true `NO_PCM` semantic for Yellow `$FF` instead of reusing the current pre-P9 modern-cry fallback;
6. keep ordinary Pikachu behavior unchanged;
7. apply original Yellow PCM only to canonical Partner Pikachu or to explicitly Yellow-owned non-field Partner events;
8. preserve P1-P8 Partner ownership/priority and accepted authored-reaction behavior;
9. wire portrait-internal cry timing separately from outer Emotion cry timing where Yellow distinguishes them;
10. require build validation and then manual gameplay/audio validation before P9 audio work is accepted.

No deferred or Not Applicable clip may be added merely for completeness during P9B.

---

# 9. P9A Closeout

**P9A is LOCKED under Option B — Reachability Policy.**

Authoritative counts:

- canonical source IDs: **42**;
- Required: **27**;
- Deferred-only: **1**;
- Not Applicable: **14**;
- initial approved PCM ROM budget: **446,892 aligned bytes (~436.42 KiB)**.

This document intentionally does not claim that the PCM binaries are already imported, that runtime playback is already wired, or that P9 audio behavior has been manually accepted. Those are P9B+ implementation/validation responsibilities.
