# Pokémon Golden Yellow — Melanie and Bulbasaur Acceptance Record

**Document type:** Dedicated gameplay acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Acceptance date:** September 8, 2026  
**Milestone:** Melanie's Cerulean Bulbasaur gift

---

# 1. Purpose

This record preserves the accepted implementation and regression boundary for the Yellow-derived Melanie/Bulbasaur gift event in `CeruleanCity_House3_Frlg`.

It supplements the Yellow Unique Specification, cumulative Acceptance Log, Current Project State, and the original implementation-notes document.

---

# 2. Acceptance Evidence

**Manual result:** PASS  
**Initial implementation:** `57a5bbe2d459d05b98fe0a62d96f83c880470626` — `feat: add Melanie Bulbasaur gift scene`  
**Strict-build correction:** `c39c33490544ffc1c229f7741abc2671b4b54c6b` — `fix: preserve debug flag for Melanie gift`  
**Final accepted source tip:** `2f478f5caca2a44d4915ddacc87aac100ec8adff` — `fix: stage Melanie gift checkpoint`

The initial strict FireRed build exposed that raw debug flag `FLAG_0x0BD` was still required. The correction restored that debug symbol and assigned Melanie's persistent hide/completion state to named flag `FLAG_HIDE_CERULEAN_HOUSE3_BULBASAUR` at `0x0BE`.

A workflow-run identifier for the final accepted source tip was not supplied in the acceptance conversation. The user explicitly confirmed the resulting gameplay as a PASS.

---

# 3. Canonical Event Rules

The accepted event:

1. occurs in Melanie's Cerulean house;
2. requires the canonical `SPECIES_PIKACHU_STARTER`;
3. reads that Partner's actual `MON_DATA_FRIENDSHIP`;
4. requires friendship `>= 147`;
5. does not permit ordinary `SPECIES_PIKACHU` to satisfy the gate;
6. gives one normal Lv. 10 Bulbasaur;
7. records one-time completion only after successful party or PC receipt.

Mood is not an eligibility requirement.

---

# 4. House Placement and Preservation

Accepted object positions are:

| Object | Coordinate | Idle facing |
|---|---:|---|
| Melanie | `(8,2)` | South |
| Bulbasaur | `(7,2)` | South |

Melanie occupies the upper-right corner and is approached from the south. Bulbasaur is immediately to her left. Their southern interaction tiles remain usable.

The following existing content is preserved:

- old man at `(2,2)` and his Jynx trade;
- old woman at `(7,5)` and her original dialogue;
- `LAYOUT_HOUSE1_FRLG`;
- the three exit warps at `(3,7)`, `(4,7)`, and `(5,7)`;
- ordinary Partner follower behavior.

---

# 5. Gift and Interaction Flow

Before adoption, Bulbasaur is visible and directly interactable. It faces the player, cries, and displays observation text without bypassing Melanie.

Melanie provides distinct branches for:

- canonical Partner unavailable;
- friendship below `147`;
- eligible offer;
- declined offer;
- party receipt;
- PC receipt;
- no available party/storage capacity;
- post-gift dialogue.

Decline and no-storage leave the gift available. Successful party or PC receipt sets the named completion flag and removes the overworld Bulbasaur.

After successful receipt, the existing Partner reaction director plays `GY_PARTNER_REACTION_CAPTURE_SUCCESS` (Reaction 18). The canonical follower faces the player before and after this authored beat.

---

# 6. Debug Checkpoint

`GY_DEBUG_CP_BULBASAUR_GIFT` is accepted as deterministic testing infrastructure.

It:

- warps to `MAP_CERULEAN_CITY` at `(15,18)`, directly outside Melanie's house entrance;
- clears `FLAG_HIDE_CERULEAN_HOUSE3_BULBASAUR`;
- reconstructs the Cerulean-era test party;
- sets canonical Partner friendship to `200/255`;
- sets canonical Partner mood to `200/255`.

The checkpoint facilitates testing but is not itself the gameplay eligibility source.

---

# 7. Regression Baseline

Future Cerulean, Charmander, Misty, follower, reaction, or checkpoint work must preserve:

1. the `147` canonical Partner friendship gate;
2. ordinary Pikachu exclusion;
3. Lv. 10 one-time Bulbasaur receipt;
4. party, PC, nickname, decline, and no-storage behavior;
5. upper-right Melanie/Bulbasaur placement and correct facings;
6. existing old-couple interactions and house exits;
7. direct pre-adoption Bulbasaur interaction;
8. completion persistence across map reload and save reload;
9. safe canonical Partner Reaction 18 behavior;
10. the corrected deterministic checkpoint.

---

# 8. Closeout

**Milestone status:** ACCEPTED

The next active scope is the reopened Route 24 Charmander scene-quality revision. That revision must preserve this Melanie/Bulbasaur milestone.
