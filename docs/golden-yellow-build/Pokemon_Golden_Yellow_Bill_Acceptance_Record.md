# Pokémon Golden Yellow — Bill Partner Pikachu Acceptance Record

**Document type:** Detailed milestone acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Original acceptance date:** August 29, 2026  
**Later P7 regression closeout:** September 1, 2026  
**Status:** ACCEPTED

---

# 1. Purpose

This record preserves the accepted implementation history and protected regression baseline for Yellow-specific Partner Pikachu behavior at Bill's Sea Cottage.

It covers two related but historically distinct acceptance layers:

1. the original transformed/restored Bill Partner reaction and Yellow portrait-renderer vertical slice accepted on August 29, 2026;
2. the later P7 authored A-button precedence, live-follower lifecycle stabilization, and complete exit-ownership closeout accepted on September 1, 2026.

The original acceptance is not replaced by the later work. The later work extends and stabilizes the same milestone while preserving the already accepted Bill transformation choreography and portrait result.

Project-wide cumulative acceptance remains in `Pokemon_Golden_Yellow_Acceptance_Log.md`.

---

# 2. Original Acceptance Evidence — Bill Transformation / Portrait Slice

**Initial portrait implementation commit:** `34d7e033dfce7077ee67d593a66a6cef5888b2d7`  
**Historical final accepted source commit:** `0cfdbc7ca43ef68ee72dabc75ea8f70834bdc694`  
**Historical final commit message:** `fix: center Yellow Pikachu portrait window`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33256305375`  
**Artifact ID:** `9715960746`  
**ROM SHA-256:** `df93fc0c69cd69e81be03a27c4baef63e5bcd5f0ea78211ee06b0e2d7f96adc0`  
**Manual result:** ACCEPTED PASS

The original acceptance established:

- retained compatible FRLG Bill progression;
- required Yellow Partner reaction to transformed Bill;
- required Yellow Partner reaction when Bill returns to human form;
- original Yellow Pikachu portrait art for both reactions;
- native GBA portrait rendering and blocking behavior;
- accepted transformed/restored timing and choreography;
- canonical Partner-only gating through `SPECIES_PIKACHU_STARTER`.

---

# 3. Original Accepted Gameplay Behavior

The accepted transformed/restored sequence is:

1. The player enters and progresses through Bill's Sea Cottage scene.
2. Partner Pikachu independently reacts to transformed Bill.
3. The transformed reaction uses Yellow's confused/questioning portrait sequence.
4. The portrait blocks scene advancement until dismissal or lifetime expiry.
5. Bill's Cell Separator sequence continues without stale portrait graphics or broken follower state.
6. After Bill returns to human form, the accepted scene pause and movement remain intact.
7. Bill moves down one tile so he reaches Partner Pikachu's row before the restored-human reaction.
8. The exclamation/cry beat occurs.
9. The restored-human reaction uses Yellow's shocked/surprised portrait sequence.
10. The scene remains blocked until the portrait closes.
11. Partner's existing excitement choreography continues.
12. The Bill sequence resolves normally.

---

# 4. Yellow Source Fidelity

Transformed-Bill reaction:

- Yellow emotion: `PikachuEmotion23`;
- Yellow portrait script: `PikaPicAnimScript23`;
- source frames:
  - `gfx/pikachu/unknown_e718f.png`;
  - `gfx/pikachu/unknown_e731f.png`.

Restored-human Bill reaction:

- Yellow emotion: `PikachuEmotion27`;
- Yellow portrait script: `PikaPicAnimScript27`;
- source frames:
  - `gfx/pikachu/unknown_f0abf.png`;
  - `gfx/pikachu/unknown_f0b64.png`.

Locked interpretation:

- transformed Bill → confused/questioning;
- restored human Bill → shocked/surprised.

The restored-human reaction must not be converted into a generic happy reaction merely because the story outcome is positive.

---

# 5. Accepted Portrait Architecture

The Bill slice established the first validated GBA carrier for Yellow's Pikachu portrait artwork and later became the basis for the generalized P1/P2 renderer.

Protected characteristics include:

- original 40×40 Yellow artwork preserved without scaling;
- native 64×64 GBA OBJ carrier;
- 4bpp indexed graphics;
- centered portrait presentation;
- script-blocking native wait state;
- safe sprite/task allocation behavior;
- A/B dismissal plus lifetime expiry;
- deterministic sprite, palette, window, and task cleanup;
- no mutation of Partner identity or save identity for portrait display.

The accepted Bill timing remains:

- confused portrait: `70 × 3 = 210` GBA frames total;
- confused frame change: `16 × 3 = 48` frames;
- shocked portrait: `30 × 3 = 90` frames total;
- shocked frame change: `4 × 3 = 12` frames.

---

# 6. P7B Accepted Manual A-Button Precedence

P7B later added authored direct Partner interaction around the already accepted Bill scene without rewriting the machine sequence.

**Accepted P7B source commit:** `29347ce1cfeb769cbf4fd900471742c6e00ff79b`  
**Workflow run:** `33473081407`  
**Artifact ID:** `9787289350`  
**ROM SHA-256:** `2fdb95915613c665ccdd5f380ad771ab63dd191c4197ca21e6d7f75e1c61374e`  
**Manual result:** PASS

Accepted manual mapping:

- before Bill is restored → Yellow Emotion `32`;
- after Bill is restored → Yellow Emotion `31`.

The automatic cutscene reactions remain:

- transformed Bill → Emotion `23`;
- restored Bill → Emotion `27`.

Manual Partner interaction does not advance Bill story flags, machine state, ticket state, or dialogue progression.

---

# 7. P7E Priority Integration

The later unified P7E interaction resolver preserves Bill as the highest-priority authored story owner when the selected object is the live Sea Cottage Partner object.

The accepted global priority is:

**Authored story**  
→ **actual status**  
→ **authored area**  
→ **one-shot modifier**  
→ **friendship/mood fallback**

Bill's manual Emotion `32` / Emotion `31` mapping remains unchanged inside this unified routing model.

---

# 8. Bill Live-Follower Lifecycle Regression and Accepted Stabilization

P7E exposed a regression in the Sea Cottage follower ownership lifecycle: the scene could otherwise leave Partner Pikachu frozen/scene-owned and prevent clean release/exit after the manual reaction.

The stabilization lineage is:

- `a060cd40bb230c8fdfa5162b44fe0262073f01d0` — `fix: stabilize Bill Partner scene lifecycle`;
- `ca001ff81a965fe1ba51d11a84e6ae19d9e73681` — `fix: place Bill interaction sentinel in EWRAM`;
- `f603bb81f354fcadf05a8c08991f8df15694ed69` — `fix: keep Bill Partner follower continuous`;
- `fa0a8e37b84bc3e491d2d101eee73a607735cda2` — `fix: release Bill Partner follower after reaction`;
- `3c35546b067ed4a4233c078935b9af3d07157fe9` — `fix: detect live Bill Partner follower`.

Accepted lifecycle result:

- the actual live follower object remains the Partner representation through the authored Sea Cottage lifecycle;
- direct A-button interaction identifies that live object correctly;
- completion of the authored release reaction restores normal follower authority;
- Partner Pikachu does not remain frozen or permanently scene-owned;
- the player is not trapped in the house after completing the required Partner interaction.

---

# 9. Accepted Exit-Ownership Geometry

The Sea Cottage uses a three-wide exterior warp row:

- left warp `(6,9)`;
- center warp `(7,9)`;
- right warp `(8,9)`.

The accepted guard layout contains exactly three coordinate events:

- center-upper guard `(7,8)`;
- left side-warp guard `(6,9)`;
- right side-warp guard `(8,9)`.

The center warp itself remains unchanged.

The first geometry correction was:

- `f37dab37a2478ce1aae8382b56c03b00d07f4846` — `fix: guard Partner scene side warp exits`.

That correctly moved the left/right guards onto the side warp tiles but still reused the same `DIR_SOUTH`-gated script for all three guards.

Manual testing proved that a player could enter a side warp laterally while facing east/west and bypass the guard.

The final correction was:

- `8005312c27b030a451b647cd3ae44f87a1bcad30` — `fix: block lateral Partner scene warp exits`.

Final accepted behavior:

- `(7,8)` remains the center-upper guard and retains the south-facing-only test;
- `(6,9)` and `(8,9)` use the direction-independent side-exit check;
- all three guards share the same existing Bill/Partner state ownership logic after the direction decision;
- Bill-in-teleporter state still blocks exit;
- post-restoration canonical Partner waiting state still blocks exit until Partner is released;
- existing warning text and corrective `walk_up` behavior are reused;
- no additional coord events or C-level exit subsystem was added.

---

# 10. Final P7 Bill Acceptance Evidence

**Final accepted source tip:** `8005312c27b030a451b647cd3ae44f87a1bcad30`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33561044203`  
**Artifact ID:** `9821412723`  
**ROM SHA-256:** `2d6f187840c0715112d91e8332854d6b2452beeb119fd1188604cd34d52b2c4b`  
**Manual result:** PASS — user confirmed the final regression build passed

This final PASS supersedes the previously open Bill follower-release/exit regression while preserving all earlier accepted Bill behavior.

---

# 11. Protected Regression Decisions

Future work must preserve:

- canonical Partner identity through `SPECIES_PIKACHU_STARTER`;
- ordinary Pikachu must never satisfy Bill Partner-only routing;
- transformed Bill → Emotion `23` / confused-questioning portrait;
- restored Bill → Emotion `27` / shocked-surprised portrait;
- Bill reaches Partner's row before the restored-human reaction;
- pre-restoration manual interaction → Emotion `32`;
- post-restoration manual interaction → Emotion `31`;
- manual Partner reactions do not mutate Bill story progression;
- the live follower object remains usable through the authored scene lifecycle;
- manual release restores normal follower authority;
- the player cannot leave while Bill is inside the machine or while the required scene-owned Partner waiting state still owns the exit;
- the center-upper guard remains south-facing-only;
- both side warp tiles remain guarded from lateral as well as southern approach while the blocking state is active;
- exactly three exit coord events remain unless a future redesign is explicitly approved;
- portrait rendering remains on the common P1/P2 backend;
- P7E single-owner priority remains authoritative.

---

# 12. Related Implementation Surfaces

Primary Sea Cottage surfaces:

- `data/maps/Route25_SeaCottage_Frlg/map.json`
- `data/maps/Route25_SeaCottage_Frlg/scripts.inc`

Partner interaction/reaction surfaces include:

- `src/golden_yellow_partner_interaction.c`
- `src/golden_yellow_partner_reaction.c`
- shared Partner state/follower infrastructure.

---

# 13. Acceptance Boundary

The Bill milestone is part of the accepted P7 authored-reaction regression baseline.

Later P8 modern follower coexistence work must layer contextual follower behavior underneath the accepted Yellow priority system without changing Bill's authored ownership, manual mappings, follower release lifecycle, or exit guards.
