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

# 12. Maintenance Rule

This log is governed by Hard Rule `DOC-006`.

When a new gameplay result is explicitly accepted:

1. append or update the relevant entry in this Acceptance Log;
2. include commit/workflow evidence when available and useful;
3. record the tested scope and important regression invariants;
4. create or update a dedicated acceptance record only if the milestone needs detailed historical preservation;
5. review Current Project State and update it only if the broad operational boundary materially changed.

The Acceptance Log must not carry a mutable "next work" or "next acceptance boundary" section. Current and future development boundaries belong in `Pokemon_Golden_Yellow_Current_Project_State.md`.

This keeps acceptance evidence durable without turning the ledger into a second project-status document.
