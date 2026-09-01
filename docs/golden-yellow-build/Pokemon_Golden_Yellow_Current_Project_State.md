# Pokémon Golden Yellow — Current Project State

**Document type:** Broad operational project snapshot  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Snapshot date:** September 1, 2026

---

# 1. Purpose

This document answers one question:

> **Where does Pokémon Golden Yellow broadly stand right now, and what is the next active development boundary?**

It is intentionally a high-level operational snapshot.

It does **not** serve as the permanent evidence record for individual gameplay acceptances, CI runs, ROM hashes, scene choreography, or fix-by-fix history.

Those details belong in:

- `Pokemon_Golden_Yellow_Acceptance_Log.md` for the cumulative acceptance history;
- dedicated acceptance records when a milestone is complex enough to require one, including `Pokemon_Golden_Yellow_MtMoon_Acceptance_Record.md`, `Pokemon_Golden_Yellow_Charmander_Acceptance_Record.md`, and `Pokemon_Golden_Yellow_Bill_Acceptance_Record.md`.

The live `prototype/v0.1` repository remains authoritative for implementation state when this snapshot becomes stale.

---

# 2. Documentation Roles

The Golden Yellow documentation set has these distinct roles:

1. `Pokemon_Golden_Yellow_Project_Master.md` — overall project architecture and campaign direction.
2. `Pokemon_Golden_Yellow_Hard_Rules.md` — implementation rules and non-negotiable constraints.
3. `Pokemon_Golden_Yellow_Yellow_Unique_Specification.md` — Yellow-specific Kanto narrative requirements.
4. `Pokemon_Golden_Yellow_FireRed_Retention_Specification.md` — compatible FireRed/Gen III systems and presentation to retain.
5. `Pokemon_Golden_Yellow_Current_Project_State.md` — broad current development position.
6. `Pokemon_Golden_Yellow_Acceptance_Log.md` — canonical cumulative manual-acceptance ledger.
7. `Yellow-portrait.md` — source audit and phased reconstruction blueprint for Yellow Partner Pikachu portraits/reactions.
8. Dedicated acceptance records — deep implementation history for major milestones when warranted.

Current State must not duplicate the Acceptance Log in detail.

---

# 3. Current Campaign Position

The initial Yellow-era Kanto campaign is in active development.

Broad progression currently stands at:

**Pallet / Yellow Opening**  
→ **Viridian / Viridian Forest**  
→ **Pewter / Brock**  
→ **Route 3 / Mt. Moon**  
→ **Cerulean / Route 24 / Route 25 / Bill**  
→ **Yellow Partner portrait catalog + generalized renderer (P1/P2) — ACCEPTED**  
→ **Yellow Partner reaction-program director (P3) — ACCEPTED**  
→ **Normal A-button Partner interaction (P4) — IMPLEMENTED**  
→ **Yellow Partner friendship/mood selection (P5) — IMPLEMENTED**  
→ **Yellow Partner one-shot reactions (P6) — IMPLEMENTED**  
→ **P7A Pewter/Jigglypuff authored integration — ACCEPTED**  
→ **P7B Bill authored A-button precedence integration — ACCEPTED**  
→ **P7C Vermilion Pokémon Fan Club authored Partner integration — NEXT STAGED BOUNDARY**

The accepted gameplay baseline extends through the Yellow Cerulean Rival #3 / Fame Checker integration, Route 24 Charmander rescue / Damian adoption milestone, the required Bill / Sea Cottage transformed/restored Partner reaction, P7A Pewter/Jigglypuff authored integration, and P7B Bill manual A-button precedence integration.

The complete directly selectable Yellow portrait-program catalog (`0–28`), reusable native-GBA portrait renderer/presentation layer, unified follower-emote resolver, and reusable Yellow reaction-program director (`0–32`) remain manually accepted foundations.

P4, P5, and P6 are implemented layers above that accepted foundation. Their presence in the live repository does not by itself create separate manual acceptances.

The immediate Partner-system engineering boundary is now the next required authored Yellow reaction integration. In campaign order, the next staged target is the Vermilion Pokémon Fan Club reaction, followed later by the Pokémon Tower fear/unease reaction.

---

# 4. High-Level Campaign Status

| Campaign area | Broad status |
|---|---|
| Yellow opening / Pallet / Oak Lab | Implemented and accepted |
| Viridian / early Route 2 / Viridian Forest | Implemented for current approved scope |
| Pewter / Brock / post-Brock progression | Implemented and accepted for current approved scope; P7A authored Partner integration accepted |
| Route 3 / Mt. Moon | Implemented and accepted |
| Cerulean / Route 24 / Route 25 / Bill | Rival/Fame Checker, Charmander rescue, required Bill cutscene reactions, and P7B Bill A-button precedence accepted; Bulbasaur/Misty remain |
| Vermilion / S.S. Anne / Lt. Surge | Next staged audit/integration area; required Fan Club Partner reaction remains |
| Lavender / Pokémon Tower | Pending staged audit and required Partner fear/unease integration |
| Celadon / Rocket Hideout | Pending staged audit and Yellow integration |
| Fuchsia / Saffron / Cinnabar / Viridian Gym | Pending staged audit and Yellow integration |
| Victory Road / Indigo Plateau | Pending staged audit and Yellow integration |
| Sevii campaign | Future campaign stage |
| Johto / Crystal campaign | Future campaign stage |
| Later Kanto return / endgame | Future campaign stage |

This table is intentionally broad. It does not replace the Acceptance Log.

---

# 5. Established Foundation Systems

The following project foundations are already present and should be built upon rather than re-created independently.

## 5.1 Canonical Partner Pikachu

Golden Yellow distinguishes:

- canonical Partner Pikachu: `SPECIES_PIKACHU_STARTER`;
- ordinary Pikachu: `SPECIES_PIKACHU`.

Partner-aware systems resolve the canonical Partner specifically rather than treating every Pikachu as the Partner.

Current architecture includes persistent Partner following, Partner-specific restrictions, Partner-aware checkpoint reconstruction, follower identity resolution by actual party Pokémon rather than assumed party slot, and Partner-only gating for Yellow narrative reactions.

## 5.2 Follower / Scripted Movement Foundation

The project has a follower-safe scripted movement mechanism for scenes that need Partner Pikachu to remain visible and continue following during controlled player/NPC choreography.

Battle-opening follower integration also uses actual resolved follower Pokémon identity so that only the Pokémon truly represented by the overworld follower receives the follower-style battle entrance.

Accepted Cerulean scenes demonstrate lane-aware, scene-local Partner staging across multi-actor choreography without replacing the generic follower engine.

P3 established native `ScriptMovement` sequencing as the accepted reusable body-language backend for Partner reactions. Reusable reaction choreography stays on the follower's current map tile and restores normal follower authority after each sequence.

P7A established repeat-safe authored Partner lifecycle handling across the Pewter Jigglypuff sleep reaction, follower-object handoff, blocker behavior, stair warps, and escalator transitions.

P7B additionally proves that the common P3 director can target a scene-owned visible Partner object when a map intentionally suppresses the generic follower, without creating a second reaction engine.

## 5.3 Yellow Rival Eevee State

Persistent Yellow Rival branching exists for:

- Jolteon path;
- Flareon path;
- Vaporeon path.

The branch is established by the Oak Lab and optional early Route 22 battles and is intended to control later Yellow Rival progression.

Later Rival encounters must be audited at their story boundary rather than assumed complete merely because dispatch infrastructure exists.

## 5.4 Trainer Watch / Time Foundation

Trainer Watch and persistent time architecture exist as the early foundation for later Crystal-era time-sensitive systems.

## 5.5 Campaign Phase Foundation

Persistent campaign-phase architecture exists for the long-form progression from Yellow Kanto into Sevii, the time transition, Johto, later Kanto, and endgame.

The state architecture exists; the later campaigns themselves are not yet implemented merely because their phase constants exist.

## 5.6 Debug / Checkpoint Foundation

Golden Yellow debug checkpoints reconstruct story state for direct event testing.

A checkpoint is testing infrastructure only.

> **Checkpoint existence must never be treated as proof that the corresponding gameplay event is implemented or accepted.**

The accepted Charmander checkpoint reconstructs the pre-event Route 24 state while deliberately placing the tester south of the event trigger rather than starting the scene immediately on warp-in.

## 5.7 Full Yellow Partner Portrait Catalog and Generalized Renderer

P1/P2 of the `Yellow-portrait.md` reconstruction blueprint are accepted.

The established portrait foundation includes:

- the complete directly selectable Yellow portrait-program catalog `0–28`;
- original Yellow 40×40 portrait artwork preserved without scaling;
- a native 64×64 GBA OBJ carrier;
- reconstructed multi-stage visible portrait timelines where the source program requires them;
- Yellow-derived timing translated to GBA timing;
- script blocking for authored portrait use;
- debug/browser access for direct program validation;
- A/B or timer dismissal where applicable;
- deterministic sprite/window/palette/task cleanup;
- centered portrait presentation;
- a tightened approximately 64×64 visible outer box with 48×48 white interior;
- a dedicated warm Pikachu-yellow/gold portrait-frame treatment that restores the player's normal configured window frame after dismissal;
- preserved compatibility with the accepted Bill transformed/restored portrait reactions.

This foundation is accepted and must not be reimplemented.

## 5.8 Yellow Partner Reaction-Program Director

P3 of the `Yellow-portrait.md` reconstruction blueprint is accepted.

The established reaction foundation includes:

- ordered reaction programs for Yellow Emotion IDs `0–32` with source-semantic command ordering;
- typed commands for cry, emote/bubble, movement, portrait, delay, turn-away, and semantic callbacks;
- canonical Partner-only gating through `SPECIES_PIKACHU_STARTER`;
- the accepted P1/P2 renderer as the single portrait backend;
- existing HGSS follower-emotion presentation for standard emotional states;
- shared field-emote presentation for Skull, Sleep/ZZZ, Bolt, Fish, and animated exclamation;
- accepted reaction-specific emote mapping across `0–32`;
- native `ScriptMovement` body-language sequencing using tile-safe follower movement scripts;
- deterministic front/side/back final pose enforcement before portrait display;
- clean restoration of follower authority after reaction movement;
- a debug reaction browser with deterministic player-facing reset and manual pre-portrait inspection hold;
- production/one-shot reaction execution that remains automatic and independent from the debug-only portrait gate;
- semantic story callbacks kept inert until an authored integration explicitly proves ownership of a story effect.

P4/P5/P6/P7 extend this common director rather than creating parallel Partner interaction, mood, modifier, or authored-reaction engines.

## 5.9 Implemented P4/P5/P6 Partner Layers

The live implementation includes:

- P4 routing of canonical Partner Pikachu A-button field interaction through the P3 director;
- P5 integration of Yellow-style friendship/mood-based normal talk selection;
- P6 integration of supported one-shot Yellow Partner reaction modifiers.

These implemented layers are infrastructure for authored P7 precedence work. They must not be treated as separately manually accepted merely because they compile or are present in the repository.

## 5.10 Accepted P7 Authored Integration Foundation

P7A and P7B are manually accepted.

P7A proves that a required Yellow authored scene can migrate onto the common Partner architecture while preserving repeat-safe scene lifecycle and normal following.

P7B proves authored A-button precedence around an already accepted scene without rewriting that scene. Its accepted behavior is:

- pre-restoration manual Sea Cottage Partner interaction → Yellow Emotion `32` through P3;
- post-restoration manual Sea Cottage Partner interaction → Yellow Emotion `31` through P3;
- automatic Bill cutscene reactions remain owned by the accepted scene;
- canonical Partner identity remains mandatory;
- the scene-owned visible Partner object can be targeted through a narrow P3 object override;
- the Bill semantic callback remains non-authoritative and manual Partner reactions do not mutate Bill story state.

Later authored P7 work must extend these accepted precedence/ownership patterns rather than fork them.

---

# 6. Established Design Decisions

These are broad project decisions, not individual acceptance records.

- Pokémon Yellow owns the initial Kanto narrative identity.
- FireRed/LeafGreen owns compatible GBA map architecture, presentation, facilities, and proven choreography where Yellow is silent.
- `pokeemerald-expansion` provides the modern systems baseline where compatible.
- Pokémon Crystal owns the later Johto narrative identity.
- TMs remain consumable; reusable TMs are not enabled.
- Modern battle systems remain active, including Natures, Abilities, IVs/EVs, held items, modern movesets, and the Physical/Special split.
- Compatible FireRed quality-of-life systems are retained unless they conflict with Yellow canon.
- Gen 1 and Gen 2 Pokémon must ultimately be obtainable in a self-contained single-player save.
- Trade-dependent evolutions must ultimately have single-player alternatives.
- Accepted gameplay should not be reopened without a regression or an explicitly approved redesign.
- Original Yellow Partner Pikachu graphics and reaction semantics may be translated onto native GBA rendering architecture rather than reproducing the Game Boy renderer literally.
- P4 and later Partner-reaction work must reuse the accepted P1/P2/P3 portrait/reaction foundation rather than creating parallel systems.
- Reusable Partner reaction movement remains tile-safe; authored scenes may use controlled displacement only when map geometry and follower safety are explicitly guaranteed.
- P7 authored integration changes reaction precedence/ownership only as narrowly as necessary; it must not casually rewrite an already accepted story scene.
- P3 semantic callbacks remain non-authoritative for story progression unless a later authored integration proves that the source reaction itself changes story state.

For the exact content of accepted milestones, use the Acceptance Log.

---

# 7. Accepted Story and Engine Boundary

The currently accepted Yellow story/engine boundary reaches through:

- Yellow opening and Partner Pikachu foundation;
- Viridian early progression and Teachy TV integration;
- Pewter/Brock and follower-safe early scripted scenes;
- required Pewter Jigglypuff Partner reaction;
- P7A Pewter/Jigglypuff authored Partner integration and repeat-safe sleep/follower lifecycle;
- Route 3 / Mt. Moon fossil progression;
- Jessie & James encounter #1;
- related follower/battle entrance integration validated after the Mt. Moon milestone;
- Cerulean Rival #3 Yellow integration;
- retained Fame Checker handoff after the Yellow Rival dialogue;
- Partner-safe Cerulean Rival approach, battle-return, and Fame Checker choreography;
- Route 24 abandoned Charmander discovery/rescue;
- Cerulean Pokémon Center Charmander treatment/escape sequence;
- healed Charmander return to Route 24;
- Damian return and approved adoption/gift resolution;
- Partner-safe Route 24/Damian choreography and associated revisit/decline state handling;
- Route 25 / Sea Cottage Bill progression for the current approved scope;
- required automatic Partner Pikachu reactions to transformed and restored Bill;
- accepted original-Yellow confused/questioning and shocked/surprised Bill portrait presentation;
- P7B Bill authored manual A-button precedence before and after restoration;
- complete Yellow portrait-program catalog `0–28` and generalized GBA portrait renderer/presentation foundation;
- complete reusable P3 Partner reaction-program director for Yellow Emotion IDs `0–32`;
- accepted standard/special Partner emote resolver;
- accepted tile-safe native follower reaction movement executor and deterministic portrait-pose synchronization.

P7B is now part of the regression baseline following explicit manual PASS.

The details, commits, workflow runs, ROM hashes, and manual-validation notes for accepted milestones are maintained in `Pokemon_Golden_Yellow_Acceptance_Log.md` and any linked dedicated acceptance record.

---

# 8. Current Active Development Boundary — P7C Vermilion Pokémon Fan Club Authored Reaction

P7A and P7B are accepted. The next staged Partner-system boundary is the required Yellow Pokémon Fan Club authored reaction in Vermilion City.

This stage should follow the same architectural rules established by P7A/P7B:

1. Re-audit the exact Pokémon Yellow Fan Club trigger, state selector, emotion/reaction ID, and any one-time or repeatable behavior before changing code.
2. Identify the Golden Yellow / FRLG equivalent map, object, flags, and existing Fan Club progression.
3. Preserve compatible FireRed map/event behavior unless Yellow canon requires an override.
4. Route the authored Partner response through `GoldenYellow_TryPartnerPikachuFieldInteraction` / the existing P3 director where the source behavior is an A-button precedence case.
5. Keep canonical `SPECIES_PIKACHU_STARTER` identity mandatory; ordinary Pikachu must never inherit Partner-only behavior.
6. Reuse the accepted P1/P2 portrait renderer, P3 reaction programs, P4 interaction routing, P5 mood system, and P6 one-shot infrastructure rather than creating a second path.
7. Keep semantic callbacks non-authoritative unless source evidence proves the Partner reaction itself changes persistent story state.
8. Protect P7A and P7B as regression baselines.
9. Treat CI/build success as implementation validation only; manual gameplay acceptance remains required.

The Pokémon Tower fear/unease authored reaction remains required after the Fan Club stage and should be integrated separately.

---

# 9. Major Yellow Requirements Still Ahead

## Partner Pikachu system

- P7C Vermilion Pokémon Fan Club authored reaction;
- Pokémon Tower fear/unease authored reaction;
- Partner evolution-refusal presentation where further authored integration is required;
- later audio fidelity work where Yellow-specific PCM/cry behavior is required and feasible.

## Cerulean era

- Melanie's Bulbasaur gift with canonical Partner happiness requirement;
- Misty Yellow battle identity validation.

## Vermilion era

- S.S. Anne Yellow Rival progression audit;
- required Pokémon Fan Club Partner Pikachu reaction;
- Lt. Surge Yellow battle identity audit;
- Officer Jenny's Squirtle gift after Lt. Surge / Thunder Badge.

## Lavender / Pokémon Tower

- required Partner fear/unease reaction;
- Jessie & James Pokémon Tower encounter integration.

## Remaining Jessie & James encounters

1. Rocket Hideout B4F;
2. Pokémon Tower 7F;
3. Silph Co. 11F.

## Later Yellow campaign requirements

Additional Yellow-specific trainer, boss, legendary, gift, and narrative differences remain to be staged through the rest of Kanto.

Moltres must remain in Victory Road under the approved Yellow canon.

---

# 10. FireRed / Modern Retention Direction

Compatible modern systems remain part of Golden Yellow, including the current approved direction for:

- Running Shoes;
- Bicycle and Fly infrastructure;
- Fame Checker;
- Quest Log / resume behavior;
- Vs. Seeker;
- modern Bag organization;
- consumable TM handling with modern TM UI;
- Berries;
- Move Tutors and Move Reminder;
- modern Pokédex and PC systems;
- Pokémon Center and Mart systems;
- Sevii Islands and Celio progression;
- Sevii Rocket storyline;
- breeding facilities;
- League rematches.

Inherited presence in the base engine does not automatically mean a feature has been specifically validated for Golden Yellow. Audit systems when they become relevant to active story work.

---

# 11. Acceptance and Regression Policy

Manual gameplay acceptance is tracked in `Pokemon_Golden_Yellow_Acceptance_Log.md`.

The rules are:

- a successful build is necessary but does not by itself create an acceptance;
- explicit user gameplay validation controls final acceptance for tested scenes/features;
- accepted behavior remains part of the regression baseline unless a defect is found or redesign is explicitly approved;
- dedicated acceptance records may preserve detailed choreography or implementation history for complex milestones;
- Current Project State should not duplicate that detailed evidence.

The accepted Bill transformation behavior, P1/P2 portrait renderer/presentation, P3 Partner reaction director, P7A Pewter authored integration, and P7B Bill authored precedence are regression targets for all later Partner-system work.

---

# 12. Current Blockers

**No confirmed blocking regression is open at this snapshot.**

P7A and P7B have both passed manual acceptance.

The Partner reaction architecture is therefore clear to advance to the next authored reaction stage. Any new work must first re-resolve live `prototype/v0.1` HEAD and audit the exact current Fan Club implementation/source behavior before code changes, per the project Hard Rules.

---

# 13. Maintenance Rule

Acceptance closeout is governed by Hard Rule `DOC-006`.

Update this document when the **broad project boundary changes**, for example:

- a major story region moves from pending to active;
- an active region becomes broadly complete;
- a major foundation system is added or replaced;
- a blocker materially changes project direction;
- the campaign moves into a new phase.

Do **not** rewrite this document for every small accepted fix.

For each explicit gameplay acceptance, update `Pokemon_Golden_Yellow_Acceptance_Log.md` instead.