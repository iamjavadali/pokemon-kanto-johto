# Pokémon Golden Yellow — Current Project State

**Document type:** Living implementation-status reference  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Working branch:** `prototype/v0.1`  
**Current verified HEAD:** `dd60b29aea3ecf3b06fb25da71e6a0002bd0d0a6`  
**HEAD message:** `fix: wait for player to face sleeping Partner`  
**Snapshot date:** August 17, 2026  
**Status basis:** Live `prototype/v0.1` code, accepted manual gameplay results, and approved Golden Yellow project decisions through the completed Pewter/Jigglypuff Partner reaction.

---

# 1. Purpose

This document is the living **Current Project State** reference for Pokémon Golden Yellow.

It records:

- what is actually implemented now;
- what is only foundation/infrastructure;
- what has been build-validated;
- what has been manually gameplay-validated;
- what has been explicitly accepted;
- which debug checkpoints exist;
- which checkpoints are only scaffolding;
- the current development boundary;
- the next story areas that remain pending.

This document is expected to become stale faster than the Master or Hard Rules documents and should therefore be updated after accepted implementation stages or material architecture changes.

---

# 2. Authority and Companion Documents

Project direction and implementation governance are defined by:

- `Pokemon_Golden_Yellow_Project_Master.md`
- `Pokemon_Golden_Yellow_Hard_Rules.md`
- `Pokemon_Golden_Yellow_Yellow_Unique_Specification.md`
- `Pokemon_Golden_Yellow_FireRed_Retention_Specification.md`

This document records **current implementation state**.

If this document conflicts with live `prototype/v0.1` source about what is implemented, the live repository wins for implementation state.

If live code appears to conflict with approved project direction, inspect the decision history rather than silently redefining the project.

---

# 3. Status Definitions

## COMPLETE

The scoped feature is implemented and wired into the current game flow.

## COMPLETE — FOUNDATION

The architecture exists and is usable, but additional future content still depends on it.

## PARTIAL

Some required behavior exists, but the full feature or story sequence is not complete.

## PENDING

The feature is required or approved but has not yet been confirmed as implemented.

## NOT AUDITED

The behavior may exist in inherited code, but its Golden Yellow integration has not yet been specifically verified.

## Validation labels

Implementation status and validation status are separate.

A feature may additionally be described as:

- **Build-Validated**
- **Automated-Test-Validated**
- **Manually Gameplay-Validated**
- **Accepted**

User manual gameplay acceptance controls the final accepted state for visual/choreographic scenes.

---

# 4. Current Development Position

The project has moved beyond the Stage 1A-only baseline.

Current accepted progression is:

**Stage 1A — Yellow opening/foundation: COMPLETE / ACCEPTED**  
→ **Early Stage 1 Kanto — Viridian through Pewter/Brock systems: substantially implemented and accepted where listed below**  
→ **Pewter Partner Jigglypuff reaction: COMPLETE / ACCEPTED**

The next major Yellow-specific development boundary is beyond Pewter, beginning with the Route 3 / Mt. Moon progression and then Cerulean-era content.

The existing Mt. Moon/Cerulean/Vermilion debug checkpoints are **testing infrastructure only** and do not imply that all corresponding Yellow events are implemented.

---

# 5. Stage 1A — Yellow Opening Foundation

**Status: COMPLETE / ACCEPTED**

The protected Stage 1A baseline includes:

1. Trainer Watch acquisition/setup.
2. Yellow Pallet/Route 1 opening.
3. Professor Oak's dedicated Pikachu capture.
4. Oak/player transition to the Lab.
5. Yellow Oak's Lab Eevee/starter scene.
6. First Rival battle.
7. Rival exit.
8. Partner Pikachu emergence.
9. Persistent Partner follower activation.
10. Partner identity/restriction foundation.
11. Yellow Rival Eevee branching architecture.
12. Campaign phase architecture.
13. Golden Yellow debug/checkpoint infrastructure.

Accepted Stage 1A behavior should not be reopened unless a new regression is observed or the user explicitly requests a change.

---

# 6. Yellow Opening / Oak Capture

**Status: COMPLETE / ACCEPTED**

The standard FireRed starter opening has been replaced by the Yellow opening.

Implemented flow:

- player approaches the Route 1 grass boundary;
- Oak interrupts and approaches;
- player enters the intended Yellow-style grass geometry before interception;
- visible Pikachu appears;
- Pikachu cry plays;
- dedicated `StartYellowPikachuCaptureBattle` begins;
- Oak is the capture actor;
- the player owns no phantom/filler Pokémon for this scene;
- Oak catches the level-5 Pikachu;
- field sequence resumes;
- Oak directs the player toward the Lab.

Key implementation surfaces include:

- `data/maps/PalletTown_Frlg/scripts.inc`
- `src/battle_setup.c`
- `src/battle_controllers.c`

The validated Oak capture battle architecture is protected from unrelated later-scene fixes.

---

# 7. Oak's Lab / First Rival / Partner Emergence

**Status: COMPLETE / ACCEPTED**

The Lab sequence follows the approved Yellow order:

- Rival is already waiting.
- Eevee is the single offered Poké Ball.
- Rival intercepts and takes Eevee.
- Oak permits Rival to keep Eevee.
- Oak gives the player the Pikachu caught outside.
- Rival challenges the player with Eevee.
- Rival exits.
- Pikachu emerges immediately.
- Oak explains Pikachu's dislike of staying in its Poké Ball.
- persistent Partner following begins.

Key implementation surfaces:

- `data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc`
- `data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts_frlg_base.inc`

The cinematic Pikachu emergence and the persistent follower system remain separate responsibilities.

---

# 8. Partner Pikachu Foundation

**Status: COMPLETE — FOUNDATION / ACCEPTED**

Canonical Partner identity:

`SPECIES_PIKACHU_STARTER`

Ordinary Pikachu identity:

`SPECIES_PIKACHU`

Current Partner foundation includes:

- persistent Partner identity;
- follower resolution by Partner identity rather than party slot;
- Partner follows even if another Pokémon is placed first in the battle party;
- suppression of Partner follower spawning before the story flag is enabled;
- Partner cannot evolve through ordinary species evolution data;
- Partner cannot be traded;
- Partner storage/daycare restrictions;
- Partner-aware checkpoint reconstruction;
- separation from ordinary Pikachu behavior.

Ordinary Pikachu remains normal and may evolve into Raichu.

Partner-specific narrative reactions are tracked separately from this foundation.

---

# 9. Yellow Rival Eevee Architecture

**Status: COMPLETE — FOUNDATION / ACCEPTED**

Persistent variable:

`VAR_YELLOW_RIVAL_EEVEE_STATE = 0x40FD`

States:

- `UNSET`
- `JOLTEON`
- `FLAREON`
- `VAPOREON`

Implemented branching:

- Oak Lab win → provisional Flareon lane.
- Oak Lab loss → Vaporeon lane.
- Early Route 22 win promotes provisional Flareon → Jolteon.
- Early Route 22 loss/skip leaves Flareon.
- Vaporeon remains Vaporeon.

Yellow Rival dispatch infrastructure is wired through retained later FRLG Rival scenes.

Confirmed dispatch locations include:

- Route 22 early;
- Cerulean;
- S.S. Anne;
- Pokémon Tower;
- Silph Co.;
- Route 22 late;
- Champion.

This is progression infrastructure; it does not automatically certify every later trainer-party datum or balance value.

---

# 10. Trainer Watch / Time Foundation

**Status: COMPLETE / ACCEPTED**

Trainer Watch is acquired from Mom before leaving home.

The system supports:

- RTC detection;
- saved-clock continuity;
- manual date/time fallback;
- date validation;
- weekday/date/time display;
- AM/PM display;
- Start Menu `WATCH` entry after acquisition.

Key implementation surfaces:

- `data/maps/PalletTown_PlayersHouse_1F_Frlg/scripts.inc`
- `src/trainer_watch.c`
- `src/start_menu.c`

The global time architecture is intended to support future Crystal-era day/night and timed systems.

---

# 11. Campaign Phase Architecture

**Status: COMPLETE — FOUNDATION**

Persistent variable:

`VAR_KANTO_JOHTO_CAMPAIGN`

Defined phases:

1. `KANTO_JOHTO_CAMPAIGN_YELLOW_KANTO`
2. `KANTO_JOHTO_CAMPAIGN_SEVII`
3. `KANTO_JOHTO_CAMPAIGN_TIME_SKIP`
4. `KANTO_JOHTO_CAMPAIGN_JOHTO`
5. `KANTO_JOHTO_CAMPAIGN_KANTO_RETURN`
6. `KANTO_JOHTO_CAMPAIGN_ENDGAME`

The zero/default state intentionally means Yellow Kanto.

A common Generation II unlock boundary also exists.

This is foundation only. Sevii → time skip → Johto → later Kanto transitions are not yet complete campaign implementations.

---

# 12. Viridian Old Man / Teachy TV

**Status: COMPLETE / ACCEPTED**

Golden Yellow intentionally avoids replaying FireRed's Old Man catching demonstration because Oak's opening already establishes catching.

Current accepted flow:

- Viridian road-block state remains compatible with FRLG.
- When the tutorial interaction becomes available, the player is stopped appropriately.
- Required forced-interaction alert completes before the scene continues.
- Old Man gives short catching advice.
- No duplicate catching battle/demo is played.
- Teachy TV is given.
- Teachy TV remains available in later reconstructed checkpoints.

Key implementation surface:

- `data/maps/ViridianCity_Frlg/scripts.inc`

Accepted implementation commit:

`ead7f4bfa3b6369206870dd9196576637175236f`

Teachy TV checkpoint persistence correction:

`cd6e54d8c9c4e3d15237d76d9675779550fcb3dc`

---

# 13. Route 2 and Viridian Forest

**Status: COMPLETE for current Golden Yellow retention decision**

Current approved direction is to retain the FRLG implementation unless a Yellow-specific conflict is found.

Viridian Forest does **not** require literal Yellow trainer-by-trainer parity.

Approved retention decisions include:

- retain FRLG Viridian Forest layout/progression;
- retain FRLG Rick's Weedle level;
- retain the two additional FRLG forest trainers;
- do not rewrite these solely for numerical Yellow parity.

This follows the broader trainer parity rule: preserve Yellow-specific narrative identity, but do not force literal parity when FRLG already preserves the major role/roster identity.

---

# 14. Pewter Pre-Brock Guide

**Status: COMPLETE / ACCEPTED**

The Pewter Guide redirect sequence is implemented with the accepted forced-interaction convention.

Automatic Guide trigger:

- player is stopped;
- player receives the `!`;
- alert completes;
- Guide dialogue begins;
- Guide escorts the player toward the Gym.

Voluntary A-button interaction:

- no forced `!`.

Partner Pikachu now remains visible and follows the player's scripted escort movement.

Key implementation surface:

- `data/maps/PewterCity_Frlg/scripts.inc`

The final follower-safe implementation uses:

`FLAG_SAFE_FOLLOWER_MOVEMENT`

and the standard Pokémon follower copy-movement path rather than a duplicated Pikachu route.

Manual validation passed for the Guide escort.

---

# 15. Safe Scripted Partner Movement

**Status: COMPLETE — FOUNDATION / ACCEPTED for current use cases**

A follower-engine integration correction was required because `lock` / scripted movement could freeze or recall Partner Pikachu.

Current behavior:

- safe scripted follower scenes can explicitly set `FLAG_SAFE_FOLLOWER_MOVEMENT`;
- the active standard Pokémon follower is allowed to continue consuming player copyable movement;
- collision handling does not recall the follower into its Poké Ball during a scene explicitly marked safe;
- no duplicate Pikachu movement route is required.

Key implementation surface:

- `src/script_movement.c`

This mechanism is validated by:

- Pewter Guide escort;
- Running Shoes Aide sequence.

Both manual tests passed.

---

# 16. Brock / Pewter Gym Retention

**Status: COMPLETE for approved Golden Yellow baseline**

Brock remains based on the retained FRLG Gym/Leader implementation.

Approved rule:

Golden Yellow does not require exact Yellow level/move parity when FRLG preserves the major trainer's story role and roster identity.

For Brock:

- Geodude + Onix is sufficient Yellow identity;
- FRLG levels/moves/AI are not rewritten solely for parity;
- Fame Checker integration is retained;
- Boulder Badge flow is retained;
- TM39 reward flow is retained.

This is an approved retention decision rather than a request to reproduce Yellow's trainer data number-for-number.

---

# 17. Running Shoes Aide

**Status: COMPLETE / ACCEPTED**

The inherited post-Brock Running Shoes sequence is retained.

Accepted interaction convention:

- Aide notices the player;
- `!` appears over the Aide;
- Aide approaches;
- Running Shoes are given;
- `FLAG_SYS_B_DASH` is enabled;
- normal progression resumes.

Partner Pikachu remains visible and follows correctly during the scripted sequence through the safe follower-movement mechanism.

Manual validation passed.

---

# 18. Pewter Pokémon Center Jigglypuff Partner Reaction

**Status: COMPLETE / ACCEPTED**

This is the first completed Yellow-specific Partner reaction outside the opening.

Trigger:

- player voluntarily talks to Jigglypuff;
- no forced `!`.

Partner gate:

- `FLAG_PARTNER_PIKACHU_FOLLOWING` must be active;
- party must contain canonical `SPECIES_PIKACHU_STARTER`;
- ordinary `SPECIES_PIKACHU` does not qualify.

Accepted scene:

1. Jigglypuff sings.
2. Full Jigglypuff song timing completes.
3. Pokémon Center BGM returns.
4. Dialogue box closes.
5. Partner Pikachu performs the sleepy movement.
6. weak Pikachu cry plays.
7. reusable sleep emote appears above Partner Pikachu.
8. player dynamically turns toward Partner Pikachu.
9. player text appears:
   **“PIKACHU looks fast asleep… Better wake it up.”**
10. text closes.
11. Pikachu performs an in-place wake/startled/hopping reaction.
12. normal Pikachu cry plays.
13. normal follower behavior resumes.

No battle `SLEEP` status is applied.

The reusable sleep icon asset is:

`graphics/field_effects/pics/emotion_sleep.png`

Reusable helper implementation:

`src/golden_yellow_sleep_emote.c`

Map implementation:

`data/maps/PewterCity_PokemonCenter_1F_Frlg/scripts.inc`

Important subsystem distinction learned and now enforced:

- Pokémon follower = `OBJ_EVENT_ID_FOLLOWER`
- NPC/human follower = `OBJ_EVENT_ID_NPC_FOLLOWER`

The Partner reaction must use the Pokémon follower object.

---

# 19. Accepted Stage 1B Partner/Follower Validation Milestone

The final accepted Pewter Jigglypuff fix is:

**Commit:** `dd60b29aea3ecf3b06fb25da71e6a0002bd0d0a6`  
**Message:** `fix: wait for player to face sleeping Partner`

Accepted build:

- workflow: `Prototype FireRed Development`
- run ID: `32089983184`
- artifact ID: `9308078970`
- artifact SHA-256: `969f6be3f7ce670b642398828c77cd3fc098352a054a046c88f8bad60b917c6e`
- ROM size: `33,554,432` bytes
- ROM SHA-256: `0b147c6209fbfa130af45cd4747d7d9880fbb555ed36a7a0486f2a90bb5084b7`

User manually tested the final player-facing fix and explicitly reported:

**PASS**

This milestone is therefore **Accepted**.

---

# 20. Golden Yellow Debug / Checkpoint Infrastructure

**Status: COMPLETE — FOUNDATION**

The project debug menu contains dedicated Golden Yellow checkpoint reconstruction.

Checkpoint behavior can restore:

- story flags/variables;
- badges;
- scene state;
- required key items/unlocks;
- party;
- storage;
- Bag baseline;
- money baseline;
- Partner Pikachu;
- follower state;
- Rival Eevee lane;
- map warp.

Checkpoint philosophy:

> Restore the minimum guaranteed canonical gameplay state required to test the target event.

A checkpoint should not automatically grant optional rods, pickups, consumables, TMs, or other non-mandatory inventory unless required for the test.

---

# 21. Current Checkpoint Menu

The live debug menu currently exposes the following Golden Yellow campaign checkpoints.

## Opening & Pewter

- Before Trainer Watch
- Before Route 1 Oak
- Oak Lab Rival
- Route 22 - Early
- Before Teachy TV
- Before Pewter Guide
- Before Jigglypuff
- Before Brock
- Before Running Shoes

## Mt. Moon & Cerulean scaffolding

- Mt. Moon Fossil
- Mt. Moon Jessie/James
- Before Misty
- Cerulean Rival
- Nugget Bridge Rocket
- Charmander Gift
- Before Bill
- Bulbasaur Gift

## Vermilion scaffolding

- S.S. Anne Rival
- S.S. Anne Captain
- Before Lt. Surge
- Squirtle Gift

## Rival-specific later checkpoints

Additional Rival checkpoint menus exist for:

- Pokémon Tower
- Silph Co.
- Route 22 late
- Champion

with Jolteon / Flareon / Vaporeon path selection where appropriate.

### Critical warning

**Checkpoint existence is not gameplay implementation.**

The Mt. Moon/Cerulean/Vermilion/later Rival checkpoints are primarily reconstruction/testing infrastructure unless the corresponding gameplay event is separately marked COMPLETE in this document.

---

# 22. Locked Current Design Decisions

The following are current approved rules.

## Narrative / source authority

- Pokémon Yellow owns initial Kanto narrative canon.
- FRLG owns GBA presentation, map architecture, and proven GBA choreography where compatible.
- Pokémon Crystal owns Johto narrative canon.
- Emerald is a systems/postgame reference.
- `pokeemerald-expansion` is the modern mechanics/system implementation authority where compatible.

## Partner identity

- canonical Partner = `SPECIES_PIKACHU_STARTER`
- ordinary Pikachu = `SPECIES_PIKACHU`
- ordinary Pikachu must never inherit Partner-only restrictions.

## TMs

**TMs remain consumable.**

Reusable TMs are not enabled.

Move Tutors may be reusable.

## Trainer parity

Exact Yellow trainer level/move parity is not universally required.

If FRLG preserves the major story role and roster identity, retain the proven FRLG battle unless a Yellow-specific narrative/roster difference requires replacement.

## Debug checkpoints

A checkpoint is testing infrastructure, not proof of implementation.

## Manual validation

The user controls gameplay acceptance.

Build success alone does not mark a scene accepted.

---

# 23. Required Yellow Content Still Pending

Unless separately implemented and accepted later, the following remain pending.

## Partner Pikachu reactions

- Bill transformed/restored reaction
- Vermilion Pokémon Fan Club reaction
- Pokémon Tower fear reaction
- any additional Yellow happiness/reaction presentation not yet audited

The Pewter Jigglypuff reaction is now complete and must be removed from pending lists.

## Yellow starter gifts

- Melanie / Bulbasaur gift with canonical happiness condition
- Damian / Charmander gift with no invented badge/happiness gate
- Officer Jenny / Squirtle gift after Lt. Surge / Thunder Badge

## Jessie & James

Required encounters:

1. Mt. Moon B2F
2. Rocket Hideout B4F
3. Pokémon Tower 7F
4. Silph Co. 11F

The existence of the Mt. Moon Jessie/James checkpoint does not mean the scene is implemented.

## Yellow-specific later Kanto story content

Still requires staged audit/implementation through:

- Mt. Moon;
- Cerulean;
- Vermilion;
- Lavender;
- Celadon;
- Fuchsia;
- Saffron;
- Cinnabar;
- Viridian Gym;
- Victory Road;
- Indigo Plateau.

## Legendary placement

Moltres must remain in Victory Road under the approved Yellow canon.

---

# 24. FireRed / Modern Systems to Retain

Compatible modern FRLG/Gen III systems should remain unless they conflict with Yellow canon.

Current retention direction includes:

- Running Shoes;
- Bike/Fly;
- Fame Checker;
- Quest Log / Resume;
- Vs. Seeker;
- modern Bag organization;
- consumable TM handling;
- Berry organization;
- Move Tutors;
- Move Reminder;
- modern Pokédex;
- modern PC / Pokémon Center / Mart systems;
- Sevii Islands;
- Celio / Network Machine progression;
- Sevii Rocket storyline;
- Four Island breeding;
- League rematches.

Inherited availability is not automatically equivalent to Golden Yellow validation; audit systems when they become relevant to active story work.

---

# 25. Current Development Boundary / Next Work

The accepted early-Kanto baseline now extends through the Pewter/Jigglypuff Partner reaction and the post-Brock Running Shoes follower choreography.

The next development pass should begin from the live repository and audit the next actual story segment rather than assuming the debug scaffolding is complete gameplay.

Expected next area:

**Route 3 → Mt. Moon**, including the Yellow-specific Mt. Moon story differences and Jessie/James encounter, followed by the Cerulean progression.

Before changing code:

1. resolve current `prototype/v0.1` HEAD;
2. inspect only the affected live map/script/engine files;
3. compare Yellow story behavior with retained FRLG implementation;
4. identify what can remain FRLG;
5. identify Yellow-only events that must be rebuilt;
6. add/confirm the pre-event checkpoint needed for direct user testing;
7. preserve all accepted Stage 1A and Pewter follower behavior.

---

# 26. Protected Known-Good Baseline

Do not reopen the following without a new regression or explicit request:

- Trainer Watch opening gate
- Route 1 Oak interception/capture
- no-party Oak capture behavior
- Oak/Lab transition
- Eevee snatch sequence
- first Rival battle
- Partner emergence
- Partner persistent identity
- Partner storage/daycare protections
- Rival Eevee branch foundation
- Viridian Teachy TV interaction
- Pewter Guide follower-safe escort
- Running Shoes follower-safe scene
- Pewter Jigglypuff sleep/wake Partner reaction
- reusable sleep-emote asset/helper
- dynamic player-facing toward the sleeping Partner

---

# 27. Update Log

## 2026-08-17 — Stage 1A baseline

Earlier snapshot established the Stage 1A Yellow opening/foundation, Partner identity, Rival Eevee architecture, Trainer Watch, campaign state, and checkpoint infrastructure.

## 2026-08-17 — Viridian / Pewter progression

Accepted:

- Viridian Old Man → Teachy TV flow without duplicate catch tutorial;
- checkpoint persistence correction;
- Pewter progression checkpoints;
- Partner-safe scripted movement during the Pewter Guide escort;
- Partner-safe scripted movement during the Running Shoes Aide sequence.

## 2026-08-17 — Pewter Jigglypuff Partner reaction

Accepted final behavior at:

`dd60b29aea3ecf3b06fb25da71e6a0002bd0d0a6`

The final scene includes:

- strict canonical Partner gating;
- sleepy movement;
- weak cry;
- reusable sleep emote;
- player turns toward Partner;
- sleep-realization dialogue;
- wake-up movement;
- normal cry;
- follower restoration.

Manual gameplay result: **PASS**.

---

# 28. Current Snapshot Summary

**Canonical branch:** `prototype/v0.1`  
**Verified HEAD:** `dd60b29aea3ecf3b06fb25da71e6a0002bd0d0a6`  
**Stage 1A:** COMPLETE / ACCEPTED  
**Viridian Teachy TV:** COMPLETE / ACCEPTED  
**Pewter Guide:** COMPLETE / ACCEPTED  
**Safe scripted Partner movement:** COMPLETE — FOUNDATION / ACCEPTED for current scenes  
**Brock retention baseline:** COMPLETE for approved parity policy  
**Running Shoes:** COMPLETE / ACCEPTED  
**Pewter Jigglypuff Partner reaction:** COMPLETE / ACCEPTED  
**Complete Yellow Kanto:** PENDING  
**Next major story area:** Route 3 / Mt. Moon → Cerulean  
**Sevii:** future campaign stage  
**Johto:** future campaign stage  
**Later Kanto:** future campaign stage  
**Mt. Silver/endgame:** future campaign stage

---

# 29. Governing Current-State Rule

> **Build forward from the accepted Yellow foundation. Do not equate checkpoints with implementation. Preserve the known-good Partner/follower behavior, keep TMs consumable, use Yellow for Kanto story authority and FRLG for compatible GBA presentation/choreography, and re-check the live affected files before each new implementation area.**
