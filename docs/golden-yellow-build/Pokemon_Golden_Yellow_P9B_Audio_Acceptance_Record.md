# Pokémon Golden Yellow — P9B Audio Acceptance Record

**Document type:** Dedicated implementation and manual-acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Acceptance date:** September 4, 2026  
**Status:** **ACCEPTED FOR CURRENT AUDIO SCOPE — reaction closeout remains open**

---

# 1. Purpose

This record preserves the implementation lineage, final runtime decisions, build evidence, and manual acceptance boundary for P9B Yellow Partner Pikachu PCM audio.

It does not declare every Partner reaction path defect-free. The user accepted the present sound balance, then reported separate reaction problems that must be handled as targeted follow-up work.

# 2. Governing Baseline

P9B implements the locked Option B reachability policy from `Pokemon_Golden_Yellow_P9A_Audio_Reachability_Audit.md`.

The governing constraints remain:

- stable semantic IDs `YELLOW_PIKACHU_CRY_01` through `YELLOW_PIKACHU_CRY_42`;
- only the 27 Required clips imported initially;
- Cry23 remains Deferred;
- 14 Not Applicable clips remain excluded;
- Yellow source `$FF` means true `NO_PCM`;
- ordinary Pokémon cry behavior remains unchanged;
- Yellow overrides remain limited to canonical Partner or explicitly Yellow-owned paths;
- outer-Emotion and portrait-internal cry timing remain separate.

# 3. Implemented Runtime

The initial P9B runtime:

- imports and compiles the 27 Required PCM assets through the existing DirectSound pipeline;
- maps the 42 stable IDs through a sparse Yellow cry table;
- plays exact Yellow PCM requests from the reaction director and portrait renderer;
- preserves true silence for unavailable or `NO_PCM` requests;
- does not replace ordinary `SPECIES_PIKACHU` cry behavior.

The final overlay correction removes Yellow PCM from the ordinary BGM duck/restore lifecycle. Background music continues at its existing level while the Yellow cry plays. A Yellow-specific completion query waits for the actual PCM player before reaction commands advance.

# 4. Audio Preparation and Balancing

The committed preparation path validates mono 8-bit PCM at 22,050 Hz and applies:

- 30 Hz high-pass filtering;
- fourth-order Butterworth low-pass filtering at 4.5 kHz;
- DC-offset removal;
- normalization to a -6 dBFS peak target.

Runtime gain uses measured energy groups instead of one global value.

| Group | Final level | Cry IDs |
|---|---:|---|
| Quiet | 115 | 17, 18, 19, 28, 29, 33, 35, 38, 39, 40 |
| Balanced | 95 | 06, 10, 20, 25, 31, 34 |
| Naturally loud | 76 | 01, 02, 03, 04, 05, 09, 11, 13, 15, 26, 37 |

These are engine volume values, not decibels.

# 5. Implementation Lineage

- `101901726a6e3f2374d06b1471b38d4464f74b67` — initial P9B Yellow Pikachu PCM runtime.
- `b334a147aa9ed5d31badcd08c3d3100a32309c9d` — first global volume reduction.
- `7ff2c611731c6f02f9a87221af8fcb958e64297a` — global level 60 test.
- `9a1718ef37abe0edef900874191c615511e4777c` — global level 40 test.
- `d72b90d8b8a8d00b14abd0c92a5755410b3eabb6` — filtered PCM preparation.
- `baea78d7c7f33b736d0dbbc02cf01137819a6881` — measured per-clip energy grouping.
- `bff9a81e7dc9119293b4c4ed9c5d74d189390f94` — raised initial tier balance.
- `2bad982cfc1f033d7e2843a1aaffc943426180cb` — non-ducking BGM overlay and actual-player completion wait.
- `24de67c6c1b7669a6293e74d85822eeea491f725` — overlay volume increase.
- `3ff7bccf6e1afa97a53928b67e463efc717687a0` — stronger tiered increase.
- `962cea4df4ecdacbb3b852e33656461ac0a53f0f` — final accepted working tiers `115 / 95 / 76`.

# 6. Final Build Evidence

**Source commit:** `962cea4df4ecdacbb3b852e33656461ac0a53f0f`  
**Workflow:** `Prototype FireRed Development`  
**Workflow run:** `33895487132`  
**Result:** PASS  
**Artifact ID:** `9945719424`  
**Artifact name:** `pokemon-kanto-johto-firered-962cea4df4ecdacbb3b852e33656461ac0a53f0f`  
**ROM SHA-256:** `7cc112a58987908aa0538a1a6ae33d85db4fbf3e0cf363717bc6271240a390a5`

# 7. Manual Acceptance

The user tested the final ROM and stated that the current result is good for now.

Accepted observations:

- BGM continues while Yellow Pikachu PCM plays over it;
- the final cry levels are sufficiently audible for the present baseline;
- the three-tier balance should remain unchanged during the next reaction fixes unless testing proves that a specific clip requires isolated correction.

# 8. Open Boundary

The user has found additional problems with Pikachu's reactions. Exact symptoms and reproduction steps have not yet been documented.

Before full P9 closeout:

1. collect each reaction defect and reproduction path;
2. reproduce it against the live `prototype/v0.1` branch;
3. identify the owning layer;
4. implement only the confirmed correction;
5. preserve the P9B audio baseline unless the defect specifically concerns audio;
6. rebuild and manually retest affected and neighboring reaction paths;
7. record explicit final acceptance.

# 9. Protected Regression Invariants

Future fixes must preserve:

- ordinary Pokémon cry behavior;
- P9A clip reachability classifications;
- true `NO_PCM`;
- outer-Emotion versus portrait-internal timing;
- non-ducking BGM overlay;
- actual-player completion waiting;
- accepted `115 / 95 / 76` volume tiers;
- the accepted P1–P8 Partner ownership and interaction baseline.
