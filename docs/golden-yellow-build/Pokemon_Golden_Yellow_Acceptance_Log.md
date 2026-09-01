# Pokémon Golden Yellow — Acceptance Log

**Document type:** Cumulative manual-acceptance ledger  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Log established:** August 26, 2026

---

# 1. Purpose

This document is the canonical cumulative record of Pokémon Golden Yellow gameplay milestones that have been explicitly validated and accepted.

Its job is different from `Pokemon_Golden_Yellow_Current_Project_State.md`.

- **Current Project State** describes where development broadly stands now.
- **Acceptance Log** records what has actually passed user gameplay validation.
- **Dedicated acceptance records** preserve deep implementation/choreography history for complex milestones when needed.

This log should be updated whenever the user explicitly reports a tested feature or scene as a pass, full pass, accepted, or equivalent final approval.

---

# 2. Acceptance Standard

A milestone is marked **ACCEPTED** only when the relevant implementation has been tested sufficiently for its intended acceptance scope and the user explicitly approves the result.

Build success alone is not acceptance.

Typical evidence may include:

- source commit;
- successful GitHub Actions workflow run;
- playable ROM artifact;
- targeted manual gameplay test;
- explicit user PASS / FULL PASS / acceptance statement.

Not every acceptance requires a dedicated long-form record.

Create a separate detailed acceptance record only when the milestone contains enough branching, choreography, graphics work, engine behavior, or regression-sensitive implementation detail to justify one.

---

# 3. Regression Rule

An accepted milestone becomes part of the project's regression baseline.

Accepted behavior should not be intentionally changed unless:

1. a regression or hidden defect is discovered;
2. a later required system exposes a genuine integration conflict; or
3. the user explicitly approves a redesign.

A later branch HEAD does not invalidate an earlier acceptance merely because additional commits have been added afterward.

---

# 4. Acceptance Index

| Milestone | Status | Detailed record |
|---|---|---|
| Stage 1A Yellow opening / Partner foundation | ACCEPTED | Consolidated historical acceptance |
| Viridian Teachy TV integration | ACCEPTED | This log |
| Pewter / follower-safe early progression | ACCEPTED | This log |
| Pewter Jigglypuff Partner reaction | ACCEPTED | This log |
| Route 3 / Mt. Moon fossil progression | ACCEPTED | `Pokemon_Golden_Yellow_MtMoon_Acceptance_Record.md` |
| Jessie & James encounter #1 | ACCEPTED | `Pokemon_Golden_Yellow_MtMoon_Acceptance_Record.md` |
| Battle follower send-out identity correction | ACCEPTED | This log |
| Cerulean Rival #3 / Fame Checker / Partner choreography | ACCEPTED | This log |
| Route 24 Charmander rescue / Damian adoption | ACCEPTED | `Pokemon_Golden_Yellow_Charmander_Acceptance_Record.md` |
| Bill / Sea Cottage Partner Pikachu reaction + Yellow portrait slice | ACCEPTED | `Pokemon_Golden_Yellow_Bill_Acceptance_Record.md` |
| Full Yellow Partner portrait catalog + generalized renderer (P1/P2) | ACCEPTED | This log |
| Yellow Partner reaction-program director (P3) | ACCEPTED | This log |
| P7A Pewter Jigglypuff authored Partner integration / sleep lifecycle | ACCEPTED | This log |
| P7B Bill authored A-button precedence integration | ACCEPTED | This log |

---

# 5. Historical Acceptance — Stage 1A Yellow Opening / Partner Foundation

**Status:** ACCEPTED  
**Acceptance type:** Consolidated historical milestone

The accepted Stage 1A baseline established the Yellow opening and core long-term systems required to build the rest of the campaign.

Accepted scope includes, at a broad level:

- Trainer Watch acquisition/setup;
- Yellow Pallet departure sequence;
- Professor Oak's dedicated Pikachu capture;
- Oak/player transition to the Lab;
- Rival taking Eevee;
- Oak giving the player the captured Pikachu;
- first Rival battle;
- Rival exit;
- Partner Pikachu emergence after the battle;
- persistent Partner follower activation;
- canonical Partner identity foundation;
- Partner evolution/storage/daycare/trade restrictions;
- Yellow Rival Eevee branching architecture;
- campaign-phase foundation;
- Golden Yellow checkpoint/debug foundation.

This acceptance predates creation of the cumulative Acceptance Log, so the log records it as a consolidated historical baseline rather than attempting to retrofit one artificial final commit onto the entire multi-commit stage.

Future regressions should be evaluated against the accepted behavior, not against an assumed single historical SHA.

---

# 6. Acceptance — Viridian Teachy TV Integration

**Status:** ACCEPTED  
**Accepted implementation:** Viridian Old Man / Teachy TV flow  
**Key source commit:** `ead7f4bfa3b6369206870dd9196576637175236f`  
**Checkpoint persistence correction:** `cd6e54d8c9c4e3d15237d76d9675779550fcb3dc`

Accepted behavior:

- the Viridian Old Man does not repeat a catching demonstration already established by Oak's opening;
- the forced interaction resolves correctly;
- the Old Man gives concise advice;
- Teachy TV is granted;
- progression continues normally;
- later reconstructed checkpoints preserve the required Teachy TV state.

Regression invariant:

> Golden Yellow must not restore the redundant mandatory Old Man catching battle unless explicitly redesigned.

---

# 7. Acceptance — Pewter / Follower-Safe Early Progression

**Status:** ACCEPTED

Accepted broad scope:

- Pewter pre-Brock Guide redirect behavior;
- Partner Pikachu remains visible and follows through the accepted scripted escort;
- retained Brock/Pewter Gym baseline under the approved trainer-parity policy;
- post-Brock Running Shoes Aide sequence;
- Partner-safe scripted movement during the Running Shoes scene;
- reusable follower-safe scripted movement foundation for scenes that explicitly enable it.

Regression invariant:

> Accepted scripted scenes must not freeze, duplicate, incorrectly recall, or strand the active Pokémon follower.

This grouped acceptance predates the cumulative log and represents the validated Pewter progression baseline rather than one single source commit.

---

# 8. Acceptance — Pewter Pokémon Center Jigglypuff Partner Reaction

**Status:** ACCEPTED  
**Final accepted source commit:** `dd60b29aea3ecf3b06fb25da71e6a0002bd0d0a6`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `32089983184`  
**Manual result:** PASS

Accepted narrative result:

- voluntary Jigglypuff interaction triggers the Yellow Partner reaction;
- only the canonical Partner Pikachu qualifies;
- Jigglypuff singing completes;
- Partner Pikachu visibly becomes sleepy;
- the sleep-emote presentation appears correctly;
- the player turns toward Partner Pikachu;
- the sleep-realization dialogue is shown;
- Partner Pikachu wakes/reacts and normal following resumes;
- no battle sleep status is applied.

Regression invariants:

- ordinary `SPECIES_PIKACHU` must not satisfy the Partner gate;
- the Pokémon follower object must be used rather than the NPC/human follower object;
- the accepted sleep/wake reaction must restore normal follower behavior afterward.

---

# 9. Acceptance — Route 3 / Mt. Moon Fossil Progression and Jessie & James Encounter #1

**Status:** ACCEPTED  
**Accepted source commit:** `d70eb1d8905e910076579959ab80c1cf25cae321`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33009718354`  
**Manual result:** FULL PASS  
**Detailed record:** `Pokemon_Golden_Yellow_MtMoon_Acceptance_Record.md`

Accepted milestone scope includes:

- Miguel fossil battle and both fossil-selection branches;
- Rocket Meowth theft of the unchosen fossil;
- Miguel chase/help sequence;
- Jessie, James, and Rocket Meowth overworld staging;
- Jessie/James encounter #1 narrative and battle;
- Partner Pikachu integration through the scene;
- post-battle fossil drop;
- accepted Team Rocket blast-off choreography;
- Miguel recovery/permission sequence;
- manual recovered-fossil pickup;
- dropped-fossil persistence if the player leaves before pickup.

The dedicated Mt. Moon acceptance record is authoritative for the detailed choreography and protected implementation decisions of this milestone.

Regression invariants include:

- Jessie and James remain on the accepted stable 32x32 overworld object graphics path;
- the accepted scene must not regress to placeholder sprites or previously rejected unsafe blast-off mechanisms;
- canonical Partner logic must continue to distinguish `SPECIES_PIKACHU_STARTER` from ordinary Pikachu.

---

# 10. Acceptance — Battle Follower Send-Out Identity Correction

**Status:** ACCEPTED  
**Source commit:** `83bf4dfc394a0ef3a7803bfbdaa464e6912f2922`  
**Workflow:** `Prototype FireRed Development`  
**Workflow run:** `33031832032`  
**Manual result:** PASS

## Scope

The battle-opening follower slide-in decision now compares the active battler against the actual Pokémon resolved by the project's existing Partner-aware follower Pokémon resolver.

The implementation intentionally reuses the authoritative follower identity system rather than adding a species-based shortcut or a second follower-identification mechanism.

## Accepted behavior

- canonical Partner Pikachu lead + visible Partner follower → follower-style slide-in / no Poké Ball throw;
- another lead Pokémon + visible Partner follower → normal Poké Ball send-out;
- ordinary Pikachu lead + visible canonical Partner follower → normal Poké Ball send-out;
- no visible follower → normal Poké Ball send-out;
- existing excluded battle types retain their prior handling.

## Regression invariants

- battle controller logic must compare actual `struct Pokemon *` identity rather than merely species or first-live party position;
- ordinary Pikachu must never be treated as canonical Partner Pikachu;
- the existing special-battle exclusions remain intact;
- later follower-selection architecture should continue to flow through the same authoritative follower Pokémon resolver where possible.

---

# 11. Acceptance — Cerulean Rival #3 / Fame Checker / Partner Choreography

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-27  
**Accepted source commit:** `d2ec7b084a0f1951fbcc275bd9ae444a32f57cfd`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33046024544`  
**Accepted artifact ID:** `9635775412`  
**Accepted ROM SHA-256:** `96ebc47ce289d512900edd0d1ba171cab7fb349cf4eaa32b9e9a0b8589f4c16e`  
**Manual result:** ACCEPTANCE PASSED

## Scope

This acceptance closes the Cerulean Rival #3 integration and retained FRLG Fame Checker handoff while preserving Golden Yellow's canonical Partner Pikachu behavior through the full scene.

## Accepted behavior

- the Cerulean Rival uses the already-established Yellow Rival #3 battle/dialogue integration;
- the Rival is spawned by the existing FRLG left/middle/right coordinate-event choreography rather than being incorrectly pre-spawned by the debug checkpoint;
- the Rival approaches in the same column as the player for all three trigger lanes;
- FireRed's Fame Checker handoff is restored after the Yellow post-battle Rival dialogue;
- Partner Pikachu remains visibly outside its Poké Ball when the encounter begins;
- Partner Pikachu remains outside through the battle return and Rival post-battle dialogue;
- Partner Pikachu stays behind the player while the Rival begins his first departure;
- only when the Rival starts returning north to give the Fame Checker does Partner Pikachu step to a known walkable adjacent trigger-lane tile;
- left trigger lane stages Partner toward the center lane;
- middle trigger lane stages Partner onto the left walkable lane;
- right trigger lane stages Partner toward the center lane;
- Partner Pikachu is never intentionally staged onto the non-walkable outer bush tiles;
- the Rival returns, gives the Fame Checker, completes the retained FRLG explanation/exit, and normal following resumes.

## Regression invariants

- the Cerulean Rival checkpoint must keep `FLAG_HIDE_CERULEAN_RIVAL` set so the live trigger script can position/spawn the Rival in the correct column;
- the scene must preserve `FLAG_SAFE_FOLLOWER_MOVEMENT` for the choreography needed to prevent scripted collisions from recalling Partner Pikachu;
- Partner Pikachu must not be recalled into its Poké Ball merely to clear the Rival's path;
- Partner step-aside timing belongs at the Rival's return-for-Fame-Checker beat, not immediately after battle;
- left/right positional branches must stage Partner only on confirmed walkable tiles;
- ordinary Pikachu must not substitute for canonical Partner Pikachu behavior;
- the retained Fame Checker acquisition must remain after the Yellow Rival dialogue unless explicitly redesigned.

---

# 12. Acceptance — Route 24 Charmander Rescue / Damian Adoption

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-27  
**Final manually accepted scene commit:** `1af6360f24b2f638fdd68bfe38bbd869690c4160`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33134429440`  
**Accepted artifact ID:** `9671564488`  
**Manual result:** PASS  
**Post-acceptance checkpoint correction:** `a4aca426e1d6e3cce57cacf171ea264001f014bd`  
**Checkpoint workflow run:** `33135465513`  
**Checkpoint artifact ID:** `9671932353`  
**Detailed record:** `Pokemon_Golden_Yellow_Charmander_Acceptance_Record.md`

## Scope

This acceptance closes Golden Yellow's expanded Yellow Route 24 Charmander event: discovery of the abandoned Charmander, Partner Pikachu concern choreography, rescue to the Cerulean Pokémon Center, healed return to Route 24, Damian's return, and final Charmander adoption.

The final checkpoint-only commit does not redefine the gameplay acceptance. The user explicitly accepted the gameplay scene on `1af6360f…`; `a4aca426…` then moved the developer checkpoint one tile south so testing no longer begins directly on the event trigger.

## Accepted behavior

- Charmander is encountered on Route 24 as a persistent Yellow-specific story object rather than being reduced to a generic gift NPC interaction;
- the approach supports the approved trigger geometry and does not require Charmander to face south during normal idle behavior;
- Charmander visibly turns toward the active interaction at the appropriate scene beat;
- Partner Pikachu is deliberately staged as part of the rescue scene rather than being recalled or allowed to collide with scripted actors;
- the weak-Charmander interaction provides the rescue decision and preserves the event if the player declines;
- accepting the rescue transitions the event into the Cerulean Pokémon Center treatment sequence;
- the temporary Pokémon Center Charmander treatment/escape choreography resolves without crossing or displacing Partner Pikachu incorrectly;
- the healed Charmander returns to Route 24 as a persistent story state;
- Partner Pikachu stages behind the player for Damian's return rather than drifting into Damian's path;
- Damian returns, explains the abandonment, and the sequence advances into the approved adoption outcome;
- final acceptance grants the Yellow Charmander gift at Lv. 10 and records completion so the event does not repeat;
- relevant decline/revisit paths preserve correct facing and state rather than corrupting the sequence;
- the developer Charmander checkpoint reconstructs the correct pre-event state and now lands at Route 24 `(7,10)`, south of the accepted trigger boundary.

## Regression invariants

- ordinary Pikachu must never substitute for canonical Partner Pikachu in Yellow-specific Partner choreography;
- Charmander must not idle facing south merely because the scripted interaction later requires south-facing dialogue;
- Route 24 approach/follower choreography must remain lane-aware and deterministic enough to avoid follower collisions or recalls;
- the Pokémon Center rescue/treatment state and Route 24 healed-return state must remain connected by persistent story state;
- Damian must not appear before the healed-return phase;
- Partner Pikachu must remain out of Damian's movement path during the return/adoption scene;
- declining a rescue/adoption prompt must not consume the one-time gift or irreversibly hide the required story objects;
- the Charmander checkpoint must remain outside the event trigger so selecting the checkpoint does not immediately start the scene before the tester has control.

The dedicated Charmander acceptance record is authoritative for the detailed state flow and protected choreography of this milestone.

---

# 13. Acceptance — Bill / Sea Cottage Partner Pikachu Reaction and Yellow Portrait Slice

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-29  
**Initial portrait implementation commit:** `34d7e033dfce7077ee67d593a66a6cef5888b2d7`  
**Final accepted source commit:** `0cfdbc7ca43ef68ee72dabc75ea8f70834bdc694`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33256305375`  
**Accepted artifact ID:** `9715960746`  
**Accepted ROM SHA-256:** `df93fc0c69cd69e81be03a27c4baef63e5bcd5f0ea78211ee06b0e2d7f96adc0`  
**Manual result:** ACCEPTED PASS  
**Detailed record:** `Pokemon_Golden_Yellow_Bill_Acceptance_Record.md`

## Scope

This acceptance closes the required Yellow Partner Pikachu Bill reaction at Sea Cottage and validates the project's first native GBA vertical slice for original Yellow Pikachu portrait graphics.

The accepted result preserves compatible FRLG Bill choreography while layering the Yellow-specific transformed/restored Partner reactions onto the scene.

## Accepted behavior

- only canonical Partner Pikachu (`SPECIES_PIKACHU_STARTER`) triggers the Bill portrait reactions;
- transformed Bill produces the original Yellow confused/questioning Partner portrait sequence;
- the transformed reaction preserves the existing cry/choreography and blocks scene advancement while the portrait is visible;
- restored Bill preserves the accepted timing where he moves down one tile and reaches Partner Pikachu's row before the restored-human reaction begins;
- the restored-human reaction produces the original Yellow shocked/surprised portrait sequence rather than a generic happy reaction;
- the portrait renderer preserves the original 40×40 Yellow artwork inside a native 64×64 GBA OBJ carrier;
- Yellow-derived frame timing is translated to GBA frame timing without overflowing the 6-bit animation-frame duration field;
- portraits can close by duration or A/B input;
- sprite, palette, window, and task resources are released cleanly after each display;
- the final portrait window is centered on the 240×160 GBA screen at the accepted `X = 10`, `Y = 5` outer coordinates;
- Bill, Partner Pikachu, and the scene continue normally after each portrait closes.

## Regression invariants

- ordinary `SPECIES_PIKACHU` must never trigger these Partner-only reactions;
- preserve the accepted Bill-to-Partner same-row timing before the restored-human reaction;
- transformed Bill remains mapped to Yellow's confused/questioning portrait art;
- restored human Bill remains mapped to Yellow's shocked/surprised portrait art;
- do not replace the restored-human reaction with a generic happy portrait;
- portrait presentation remains centered;
- the scene must remain blocked while the portrait is active;
- portrait rendering must not mutate follower identity or Partner save identity;
- runtime sprite/window/palette/task resources must continue to be cleaned up deterministically.

The dedicated Bill acceptance record is authoritative for the Yellow source assets, translated timing, renderer architecture, accepted scene choreography, and protected implementation decisions of this milestone.

---

# 14. Acceptance — Full Yellow Partner Portrait Catalog and Generalized Renderer (P1/P2)

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-30  
**Generalized renderer commit:** `7d2d0186691c491f1bbacc4ccd114e187401ec7c`  
**Final accepted presentation commit:** `c8d03846f5afbce26e6d55f3dd0180c166b504b5`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33321231681`  
**Accepted artifact ID:** `9734996200`  
**Accepted ROM SHA-256:** `836d93a2c3647f17eb37e864196775bdb9d84dd135aa7551ffe68495a029cbda`  
**Manual result:** PASS

## Scope

This acceptance closes P1/P2 of the Yellow Partner Pikachu portrait reconstruction: the complete directly selectable Yellow portrait-program catalog and the reusable native-GBA renderer/presentation layer that displays it.

The acceptance is based on cumulative manual review of the portrait debug/browser path across programs `0–28`, followed by a final ROM retest after the visible frame-size and Pikachu-themed border correction.

## Accepted behavior

- Yellow portrait programs `0–28` are available through the generalized portrait registry/browser path;
- the imported Yellow artwork remains unscaled 40×40 source-derived portrait content carried inside the existing native 64×64 GBA OBJ;
- multi-stage portrait programs render their reconstructed visible timelines rather than degrading to a single static frame;
- program-to-program transitions do not visibly leak stale tiles or previous portrait content;
- the debug portrait browser can move through the catalog and close back to field control cleanly;
- the visible portrait box is reduced to an approximately 64×64 outer presentation with a 48×48 white interior around the 40×40 Yellow artwork;
- the portrait frame uses a dedicated warm Pikachu-yellow/gold palette while the portrait UI is active;
- the player's normal configured window-frame graphics/palette are restored after portrait dismissal so unrelated menus are not permanently recolored;
- the portrait remains centered and the original 64×64 OBJ carrier/resource model is preserved;
- accepted Bill transformed/restored portrait behavior remains the regression baseline for authored scene integration.

## Regression invariants

- do not scale, stretch, or redraw the original 40×40 Yellow portrait artwork merely to fill the carrier;
- do not enlarge the visible box back to the prior oversized 8×8-tile interior presentation without explicit redesign approval;
- keep the Pikachu-yellow/gold frame treatment local to the Partner portrait UI and restore the user's normal frame palette afterward;
- preserve the 64×64 native OBJ carrier and deterministic sprite/window/palette/task cleanup architecture;
- portrait programs `0–28` must remain addressable in the established order and must not be silently remapped by future reaction-director work;
- ordinary Pikachu must not gain canonical Partner-only authored reaction behavior merely because the portrait renderer is reusable;
- P3 and later reaction-selection work must build on this accepted renderer rather than create a second portrait system.

This acceptance closes the graphics/catalog and generalized portrait-renderer foundation. It does not by itself accept the still-pending Yellow emotion/reaction-director selection logic, mood system, or remaining authored reaction integrations.

---

# 15. Acceptance — Yellow Partner Reaction-Program Director (P3)

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-30  
**Initial P3 director commit:** `a36b413fd837f9ae982486b48bbcf304d9495f9c`  
**Unified follower-emote implementation:** `50772d5cbfee4b0a443d0d5369af21fd46ae3a0b`  
**Follower-message-pool include correction:** `653823791b79540acd53a4f33b55840ed55ce2aa`  
**Portrait-synchronized choreography commit:** `668aaba61d728535fa4431af3a8d08ba62feae5f`  
**Debug facing normalization:** `1d1eaf4bae89f9ccc1d9157c98b457ebfdfc497a`  
**Debug portrait-gate commit:** `019728c9a7ed1f210210901a5d85cbd025685ce6`  
**Native movement executor commit:** `68224bd0eb71b65f1331a203647b301bdbd97f27`  
**Final accepted linker correction / source tip:** `598dc0534297c5a7edb099d42a857e2c0eea130a`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33342904667`  
**Accepted artifact ID:** `9741152434`  
**Accepted ROM SHA-256:** `27dee98c48112a7fa15b46c986ea42e711f9373ef049eeea82a2d09e77d6642f`  
**Manual result:** PASS — user confirmed the completed P3 result and readiness for P4

## Prior sub-acceptance — unified follower emote layer

Before full P3 closeout, the user explicitly accepted the Fix1d emote layer.

Evidence:

- implementation commit `50772d5cbfee4b0a443d0d5369af21fd46ae3a0b`;
- include-fix commit `653823791b79540acd53a4f33b55840ed55ce2aa`;
- workflow run `33337676908`;
- artifact ID `9739598594`;
- ROM SHA-256 `bfe10ac0f34b82ce9d55754d1b323e8bc1f231906f1823e51d7c344a6ec24301`;
- manual result: PASS for the emote layer.

That earlier acceptance is preserved here as a sub-acceptance and is now also part of the fully accepted P3 system.

## Scope

This acceptance closes P3 of the `Yellow-portrait.md` reconstruction blueprint: the reusable Yellow Partner reaction-program director that sequences Yellow reaction semantics over the accepted P1/P2 portrait renderer and the existing follower system.

The final manual pass was performed through the dedicated 0–32 reaction debug browser after adding deterministic test normalization and a pre-portrait inspection gate. The accepted build uses the native `ScriptMovement` executor for visible follower body-language choreography rather than the earlier unreliable direct held-movement implementation.

## Accepted behavior

- Yellow Emotion/reaction IDs `0–32` are represented by ordered, typed reaction programs and remain traceable to the source command ordering;
- reaction programs can sequence cry, follower emote/bubble, movement, portrait, delay, turn-away, and semantic callback commands;
- reaction ID `33` remains an inaccessible sentinel rather than a normal selectable reaction;
- only canonical Partner Pikachu (`SPECIES_PIKACHU_STARTER`) qualifies for the Partner reaction director;
- ordinary `SPECIES_PIKACHU` remains ordinary and is not treated as the Partner;
- the accepted P1/P2 portrait renderer is reused rather than replaced by a parallel portrait system;
- the unified emote resolver uses the existing HGSS follower-emotion presentation for standard emotional states and the shared Golden Yellow field-emote path for Skull, Sleep/ZZZ, Bolt, Fish, and the animated exclamation reaction;
- the reaction-specific 0–32 emote mapping accepted during Fix1d remains the regression baseline;
- expressive overworld movement now executes through the native scripted-movement system used by field `applymovement`, including C-linkable tile-safe equivalents of the required existing follower movement scripts;
- reaction movement preserves the follower's map tile and does not use generic tile-displacing choreography in the reusable P3 browser/program layer;
- movement completion restores follower authority rather than leaving Partner Pikachu frozen or stranded;
- deterministic final portrait-facing is enforced after expressive movement so front-, side-, and back-facing portrait semantics can agree with the overworld pose;
- the debug browser returns Pikachu to a player-facing baseline after every completed reaction so sequential testing remains deterministic;
- the debug browser holds the final overworld pose before the portrait and waits for A, allowing movement and final facing to be inspected before the portrait covers the field;
- production/one-shot reactions remain automatic and do not inherit the debug-only manual portrait gate;
- A/B portrait dismissal and reaction cleanup return control without duplicate followers, stuck held movement, or follower drift;
- source-semantic story callbacks remain inert at the reusable P3 layer and do not independently advance authored Pewter, Bill, or Fan Club story state.

## Regression invariants

- do not replace or fork the accepted P1/P2 portrait renderer for later reaction work;
- do not regress standard follower emotions to the rejected placeholder/custom presentation when the existing HGSS follower sheet already supplies the accepted emotion;
- preserve the accepted Skull, Sleep/ZZZ, Bolt, Fish, and animated-exclamation shared field-emote behavior;
- preserve the accepted reaction ID `0–32` ordering and source-semantic program mapping;
- ordinary Pikachu must never satisfy canonical Partner-only reaction gates;
- keep reusable P3 reaction choreography tile-safe unless a later authored scene explicitly guarantees safe tile displacement;
- do not reintroduce the rejected per-step direct held-movement executor as the default P3 body-language backend; the accepted path is native `ScriptMovement` sequencing;
- final portrait-facing must remain deterministic and synchronized with the intended front/side/back reaction semantics;
- debug-only test normalization/manual portrait gating must not leak into production story reaction flow;
- P3 semantic callbacks must remain non-authoritative for story progression until their authored P7 integrations explicitly bind them;
- later P4/P5/P6/P7 layers must extend this accepted reaction director rather than creating a second Partner-reaction engine.

This acceptance advances the Partner-reaction reconstruction boundary to P4: normal A-button Partner interaction on top of the accepted P1/P2/P3 foundation.

---

# 16. Acceptance — P7A Pewter Jigglypuff Authored Partner Integration

**Status:** ACCEPTED  
**Acceptance date:** 2026-09-01  
**Final accepted source tip:** `360470d8f34411b273ac584374c796c4248ef57a`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33469168689`  
**Manual result:** PASS — user explicitly confirmed P7A is accepted

## Scope

P7A closes the authored Pewter Pokémon Center / Jigglypuff migration onto the common Golden Yellow Partner reaction architecture. It preserves the previously required Yellow sleep/wake story result while integrating its lifecycle with the accepted P3 reaction director and the later P4/P5/P6 Partner systems rather than maintaining a parallel reaction engine.

The accepted source tip includes the cumulative P7A stabilization work through the follower-object handoff, repeat-safe sleep lifecycle, movement cleanup, Pewter blocker behavior, stair-warp behavior, escalator behavior, and final follower event-object constant correction.

## Accepted behavior

- voluntary Jigglypuff interaction still triggers the required Yellow Partner sleep sequence;
- only canonical Partner Pikachu (`SPECIES_PIKACHU_STARTER`) qualifies for the authored reaction;
- the P3 reaction director remains the shared reaction backend rather than a duplicated Pewter-specific emotion engine;
- the sleep lifecycle can be entered, completed, and repeated without deadlocking follower movement;
- Partner movement state is normalized at the scene boundaries so later following remains usable;
- Partner Pikachu remains visibly represented through the accepted Pewter blocker behavior;
- Partner Pikachu remains out through the corrected stair-warp and escalator cases covered by the accepted P7A build;
- the sleep/wake sequence returns normal follower authority after the authored reaction completes;
- ordinary Pikachu and unrelated followers do not inherit the canonical Partner-only authored behavior.

## Regression invariants

- preserve `SPECIES_PIKACHU_STARTER` as the canonical Partner gate;
- do not recreate a second Pewter reaction engine outside the accepted P3 director;
- do not reintroduce the follower movement deadlock or stale movement state corrected during P7A;
- preserve the repeat-safe sleep lifecycle and successful restoration of normal following;
- preserve the accepted visibility behavior at Pewter blockers, stair warps, and escalators;
- do not let P7B or later authored reactions regress the accepted P7A object/follower lifecycle.

This acceptance closes P7A. It does not accept P7B or any later authored Partner integration merely because those stages build on the same reaction director.

---

# 17. Acceptance — P7B Bill Authored A-Button Precedence Integration

**Status:** ACCEPTED  
**Acceptance date:** 2026-09-01  
**Accepted source commit:** `29347ce1cfeb769cbf4fd900471742c6e00ff79b`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33473081407`  
**Accepted artifact ID:** `9787289350`  
**Accepted ROM SHA-256:** `2fdb95915613c665ccdd5f380ad771ab63dd191c4197ca21e6d7f75e1c61374e`  
**Manual result:** PASS — user explicitly confirmed the P7B manual test passed

## Scope

P7B closes Yellow's authored Bill-house **manual A-button reaction precedence** around the already accepted Golden Yellow Sea Cottage transformation scene. It is an authored interaction integration, not a rewrite of Bill's transformation, Cell Separator sequence, machine choreography, map geometry, field-effect behavior, portrait rendering, or P3 reaction data.

The source-exact Yellow state selector distinguishes the automatic cutscene-owned Emotion `23` and Emotion `27` states from the manually reachable pre-/post-restoration A-button states. Golden Yellow preserves the accepted automatic cutscene reactions and maps the manual states through the existing P3 director as Emotion `32` before Bill's completed restoration and Emotion `31` after restoration.

## Accepted behavior

- the already accepted transformed-Bill and restored-Bill automatic scene choreography remains unchanged;
- before Bill has been helped/restored, pressing A on the visible Sea Cottage Partner Pikachu dispatches the existing P3 Emotion `32` reaction;
- after Bill has been helped/restored, pressing A on the visible Sea Cottage Partner Pikachu dispatches the existing P3 Emotion `31` reaction;
- the manual state boundary uses Golden Yellow's existing `FLAG_HELPED_BILL_IN_SEA_COTTAGE` state without changing the Bill machine sequence;
- only the canonical Partner Pikachu qualifies for the authored Bill override; ordinary Pikachu cannot qualify merely because an overworld object uses Pikachu graphics;
- the Sea Cottage scene-owned visible Partner object is routed through the same P3 reaction director rather than a second Bill-specific reaction engine;
- P3 reaction programs, emotes, movement data, portrait programs, and the portrait renderer remain shared and unchanged for this integration;
- the Bill semantic callback remains non-authoritative and the Partner A-button reaction does not advance or mutate Bill story flags, machine state, dialogue progression, or ticket state;
- repeated manual Partner interactions do not consume or alter Bill progression;
- leaving Sea Cottage restores normal Partner A-button interaction behavior outside the authored Bill override;
- the accepted P7A Pewter/Jigglypuff reaction and follower lifecycle remain intact as a regression baseline.

## Regression invariants

- preserve Yellow's manual Bill-state mapping: pre-restoration manual interaction → Emotion `32`; post-restoration manual interaction → Emotion `31`;
- preserve the existing automatic Bill cutscene-owned reactions corresponding to Yellow Emotion `23` and Emotion `27`;
- do not rewrite the accepted Bill transformation / Cell Separator choreography as part of later Partner interaction work;
- do not bind the P3 Bill semantic callback to story progression unless direct source evidence proves that the reaction itself must mutate story state;
- preserve canonical Partner identity through `SPECIES_PIKACHU_STARTER`; graphics identity alone must never authorize Partner-only behavior;
- keep the P7B scene-object target override narrowly scoped to authored reaction targeting and do not turn it into a parallel follower or movement system;
- preserve the accepted P1/P2 portrait renderer and P3 reaction director as the common backend;
- later P7 integrations must not regress P7A sleep/follower lifecycle or P7B Bill precedence behavior.

This acceptance closes P7B. The next authored P7 integrations are the required Vermilion Pokémon Fan Club reaction and Pokémon Tower fear/unease reaction, to be staged separately.

---

# 18. Maintenance Rule

This log is governed by Hard Rule `DOC-006`.

When a new gameplay result is explicitly accepted:

1. append or update the relevant entry in this Acceptance Log;
2. include commit/workflow evidence when available and useful;
3. record the tested scope and important regression invariants;
4. create or update a dedicated acceptance record only if the milestone needs detailed historical preservation;
5. review Current Project State and update it only if the broad operational boundary materially changed.

The Acceptance Log must not carry a mutable "next work" or "next acceptance boundary" section. Current and future development boundaries belong in `Pokemon_Golden_Yellow_Current_Project_State.md`.

This keeps acceptance evidence durable without turning the ledger into a second project-status document.