# Pokémon Golden Yellow — Bill Partner Pikachu Acceptance Record

**Document type:** Detailed milestone acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Branch:** `prototype/v0.1`  
**Initial portrait implementation commit:** `34d7e033dfce7077ee67d593a66a6cef5888b2d7`  
**Final accepted source commit:** `0cfdbc7ca43ef68ee72dabc75ea8f70834bdc694`  
**Final accepted commit message:** `fix: center Yellow Pikachu portrait window`  
**Accepted workflow:** `Prototype FireRed Development`  
**Accepted workflow run:** `33256305375`  
**Accepted artifact ID:** `9715960746`  
**Accepted ROM SHA-256:** `df93fc0c69cd69e81be03a27c4baef63e5bcd5f0ea78211ee06b0e2d7f96adc0`  
**Acceptance date:** 2026-08-29  
**Manual gameplay result:** ACCEPTED PASS

---

# 1. Purpose

This record preserves the detailed accepted implementation state for the Yellow-specific Partner Pikachu reaction during Bill's transformed/restored Sea Cottage sequence.

It is a deep milestone record, not the project's cumulative acceptance ledger.

Project-wide acceptance history is maintained in:

`Pokemon_Golden_Yellow_Acceptance_Log.md`

This document should only be updated when the accepted Bill milestone itself needs clarification, regression notes, or an explicitly approved redesign.

---

# 2. Accepted Scope

The accepted milestone covers:

- Route 25 / Sea Cottage Bill progression using the retained FRLG scene foundation;
- the required Yellow Partner Pikachu reaction to transformed Bill;
- the required Yellow Partner Pikachu reaction when Bill returns to human form;
- preservation of the previously accepted timing where restored Bill moves down one tile and reaches Partner Pikachu's row before the restored-human reaction begins;
- original Pokémon Yellow Pikachu portrait source graphics for the Bill reactions;
- a native GBA portrait carrier/renderer for those Yellow assets;
- Yellow-derived portrait animation timing translated to GBA frames;
- script blocking while a portrait is active;
- A/B dismissal plus automatic duration expiry;
- deterministic sprite/window/palette/task cleanup;
- centered portrait presentation on the 240×160 GBA display;
- canonical Partner Pikachu gating using `SPECIES_PIKACHU_STARTER` rather than ordinary Pikachu.

---

# 3. Accepted Gameplay Behavior

The user manually tested the full scene, first identified the portrait-window alignment defect, then tested the centered correction and explicitly accepted the result.

The accepted visible behavior is:

1. The player enters and progresses through Bill's existing Sea Cottage sequence.
2. When Partner Pikachu reacts to transformed Bill, the normal scene cry/choreography is preserved and the Yellow confused/questioning portrait is shown.
3. The confused portrait uses the original Yellow Bill reaction art and advances to its second Yellow frame at the translated timing.
4. The scene remains blocked until the portrait closes by timer or A/B input.
5. Bill's transformation sequence continues without stale portrait graphics or broken follower state.
6. After Bill returns to human form, the accepted scene pause and Bill movement remain intact.
7. Bill walks down one tile so that he reaches Partner Pikachu's row before the restored-human reaction.
8. The existing exclamation/cry beat occurs, followed by the Yellow shocked/surprised portrait.
9. The shocked portrait advances to the radiating second Yellow frame at the translated timing.
10. The scene remains blocked until the portrait closes.
11. The existing Partner excitement movement then continues and the Bill scene resolves normally.
12. Both portrait windows are centered on the GBA display.

---

# 4. Yellow Source Fidelity

The accepted Bill portrait slice uses original Pokémon Yellow Pikachu portrait assets rather than newly invented reaction art.

Transformed-Bill reaction:

- Yellow emotion: `PikachuEmotion23`;
- Yellow animation script: `PikaPicAnimScript23`;
- source frames:
  - `gfx/pikachu/unknown_e718f.png`;
  - `gfx/pikachu/unknown_e731f.png`.

Restored-human Bill reaction:

- Yellow emotion: `PikachuEmotion27`;
- Yellow animation script: `PikaPicAnimScript27`;
- source frames:
  - `gfx/pikachu/unknown_f0abf.png`;
  - `gfx/pikachu/unknown_f0b64.png`.

The semantic interpretation locked by this milestone is:

- transformed Bill → confused/questioning;
- restored human Bill → shocked/surprised.

The restored-human reaction is not to be relabeled as a generic happy portrait merely because the overall event outcome is positive.

---

# 5. Accepted GBA Rendering Architecture

The Bill slice establishes the first validated GBA carrier for Yellow's Pikachu portrait artwork.

Accepted implementation characteristics:

- original 40×40 Yellow art is centered unchanged inside a native 64×64 GBA OBJ carrier;
- the sprite uses 4bpp graphics and a four-shade grayscale presentation consistent with the Yellow source art;
- the portrait appears inside a standard bordered 8×8-tile window;
- accepted outer portrait-window coordinates are `X = 10`, `Y = 5`;
- these coordinates center the portrait presentation on the 240×160 GBA screen;
- portrait graphics use dynamic OBJ tile allocation through the sprite image path;
- `CreateSpriteUnchecked` is used so sprite-slot exhaustion can fail safely rather than hitting the fatal assertion used by `CreateSprite`;
- task capacity is checked before creating the portrait task;
- the map script remains blocked through native-script wait state until the portrait task has destroyed its runtime resources;
- cleanup destroys the portrait sprite, frees its palette, removes the window, schedules the tilemap copy, and destroys the portrait task.

This architecture is accepted for the Bill vertical slice and is the proven starting point for the upcoming full Yellow Partner Pikachu graphics/reaction-system reconstruction.

---

# 6. Accepted Animation Timing

Yellow's portrait interpreter advances once per `Delay3`, so the Bill portrait timing is translated at three GBA frames per Yellow interpreter tick.

Accepted timing:

- confused Bill portrait total duration: `70 × 3 = 210` GBA frames;
- confused frame replacement occurs after `16 × 3 = 48` GBA frames;
- shocked Bill portrait total duration: `30 × 3 = 90` GBA frames;
- shocked frame replacement occurs after `4 × 3 = 12` GBA frames.

Because `AnimFrameCmd.duration` is a 6-bit field, the full 210/90-frame lifespan is owned by the task timer rather than incorrectly encoded into one animation-frame duration.

The animation command sequence changes to the final source frame and leaves that frame displayed while the task owns the remainder of the portrait lifespan.

---

# 7. Protected Regression Decisions

The following are part of the accepted Bill regression baseline unless a defect is discovered or the user explicitly approves a redesign:

- preserve the retained FRLG Bill sequence where it is compatible with Yellow;
- preserve the accepted restored-Bill timing: Bill reaches Partner Pikachu's row before the restored-human reaction begins;
- do not move the restored reaction back to an earlier Bill position;
- transformed Bill uses the Yellow confused/questioning portrait source;
- restored human Bill uses the Yellow shocked/surprised portrait source;
- do not replace the restored reaction with a generic happy face;
- portrait presentation remains centered on the GBA screen;
- ordinary `SPECIES_PIKACHU` must never satisfy the Partner-only Bill gate;
- the portrait system must not mutate follower identity or save-state identity merely to display the reaction;
- portrait runtime resources must be released deterministically after every display;
- the scene must remain blocked while a portrait is active so Bill/follower choreography cannot advance behind it.

---

# 8. Related Implementation Surfaces

Primary Sea Cottage script surface:

- `data/maps/Route25_SeaCottage_Frlg/scripts.inc`

Partner portrait renderer:

- `src/golden_yellow_partner_reaction.c`

Imported Bill portrait carriers:

- `graphics/golden_yellow/pikachu_portraits/bill_confused_0.png`
- `graphics/golden_yellow/pikachu_portraits/bill_confused_1.png`
- `graphics/golden_yellow/pikachu_portraits/bill_shocked_0.png`
- `graphics/golden_yellow/pikachu_portraits/bill_shocked_1.png`

---

# 9. Project-State Relationship

This acceptance closes the required Yellow Bill Partner reaction milestone at the current Cerulean story boundary.

It also proves a narrow portrait-rendering vertical slice that can now be generalized deliberately rather than duplicated event-by-event.

The next active engineering boundary is the planned import of Yellow's complete Pikachu portrait/graphics catalog and reconstruction of the reusable Yellow Partner reaction/emotion system on GBA architecture.

That future system work must preserve this accepted Bill behavior while expanding beyond it.

`Pokemon_Golden_Yellow_Current_Project_State.md` should summarize this boundary broadly rather than duplicating the implementation detail in this record.

`Pokemon_Golden_Yellow_Acceptance_Log.md` carries the cumulative project-wide acceptance entry and links back here.

---

# 10. Validation Rule

Build success alone did not make this milestone accepted.

The milestone is accepted because:

- the initial portrait implementation compiled successfully;
- the initial ROM was manually tested;
- runtime testing exposed the left-shifted portrait-window defect;
- the geometry was corrected from `X = 5`, `Y = 4` to `X = 10`, `Y = 5`;
- the corrected workflow run passed;
- the corrected ROM artifact was manually tested through the full Bill scene;
- the user explicitly reported: `This is now an accepted pass.`

The final accepted source commit remains the historical milestone reference even as later development commits advance the branch.
