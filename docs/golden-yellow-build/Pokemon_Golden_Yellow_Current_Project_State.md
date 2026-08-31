# Pokémon Golden Yellow — Current Project State

**Document type:** Broad operational project snapshot  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Snapshot date:** August 30, 2026

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
→ **Cerulean / Route 24 / Route 25 / Bill — ACCEPTED THROUGH BILL REACTION**  
→ **Yellow Partner portrait catalog + generalized renderer (P1/P2) — ACCEPTED**  
→ **Yellow Partner reaction-program director (P3) — ACCEPTED**  
→ **Normal A-button Partner interaction (P4) — ACTIVE ENGINEERING BOUNDARY**

The accepted gameplay baseline extends through the Yellow Cerulean Rival #3 / Fame Checker integration, Route 24 Charmander rescue / Damian adoption milestone, and the required Bill / Sea Cottage Partner Pikachu transformed/restored reaction.

The complete directly selectable Yellow portrait-program catalog (`0–28`), reusable native-GBA portrait renderer/presentation layer, unified follower-emote resolver, and reusable Yellow reaction-program director (`0–32`) are now manually accepted foundations.

The immediate next engineering boundary is P4 from `Yellow-portrait.md`: route normal field A-button interaction with the canonical Partner Pikachu through the accepted P3 reaction director rather than creating a second interaction/reaction path.

---

# 4. High-Level Campaign Status

| Campaign area | Broad status |
|---|---|
| Yellow opening / Pallet / Oak Lab | Implemented and accepted |
| Viridian / early Route 2 / Viridian Forest | Implemented for current approved scope |
| Pewter / Brock / post-Brock progression | Implemented and accepted for current approved scope |
| Route 3 / Mt. Moon | Implemented and accepted |
| Cerulean / Route 24 / Route 25 / Bill | Active — Rival/Fame Checker, Charmander rescue, and required Bill Partner reaction accepted; Bulbasaur/Misty remain |
| Vermilion / S.S. Anne / Lt. Surge | Pending staged audit and Yellow integration |
| Lavender / Pokémon Tower | Pending staged audit and Yellow integration |
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

P3 additionally established native `ScriptMovement` sequencing as the accepted reusable body-language backend for Partner reactions. Reusable reaction choreography stays on the follower's current map tile and restores normal follower authority after each sequence.

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
- preserved compatibility with the already-accepted Bill transformed/restored portrait reactions.

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
- semantic story callbacks kept inert until later authored integration owns their story effects.

P4/P5/P6/P7 must extend this common director rather than creating parallel Partner interaction, mood, modifier, or authored-reaction engines.

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

For the exact content of accepted milestones, use the Acceptance Log.

---

# 7. Accepted Story and Engine Boundary

The currently accepted Yellow story/engine boundary reaches through:

- Yellow opening and Partner Pikachu foundation;
- Viridian early progression and Teachy TV integration;
- Pewter/Brock and follower-safe early scripted scenes;
- required Pewter Jigglypuff Partner reaction;
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
- required Partner Pikachu reaction to transformed Bill;
- required Partner Pikachu reaction when Bill returns to human form;
- accepted original-Yellow confused/questioning and shocked/surprised Bill portrait presentation;
- complete Yellow portrait-program catalog `0–28` and generalized GBA portrait renderer/presentation foundation;
- complete reusable P3 Partner reaction-program director for Yellow Emotion IDs `0–32`;
- accepted standard/special Partner emote resolver;
- accepted tile-safe native follower reaction movement executor and deterministic portrait-pose synchronization.

The details, commits, workflow runs, ROM hashes, and manual-validation notes for these milestones are maintained in `Pokemon_Golden_Yellow_Acceptance_Log.md` and any linked dedicated acceptance record.

---

# 8. Current Active Development Boundary — P4 Normal A-Button Partner Interaction

The Cerulean-era story pass remains active, but the immediate engine work is now the next layer above the accepted P3 reaction director.

The active engineering boundary is:

**accepted P1/P2 portrait renderer + accepted P3 reaction director → normal A-button Partner interaction dispatch (P4) → later P5 mood/P6 modifiers/P7 authored migration**

P4 must build directly on the accepted P3 director and the source mapping documented in `Yellow-portrait.md`.

Expected P4 scope includes:

1. Audit the existing follower A-button interaction path and identify the narrowest authoritative hook for canonical Partner Pikachu.
2. Route normal field interaction with the canonical Partner through the accepted Yellow reaction director rather than adding a second portrait/emote/movement implementation.
3. Preserve ordinary follower interaction behavior for non-Partner Pokémon unless a later project requirement explicitly generalizes the Yellow system.
4. Keep ordinary `SPECIES_PIKACHU` distinct from canonical `SPECIES_PIKACHU_STARTER`.
5. Preserve existing follower lock/release, map-event, field-control, and collision ownership rather than bypassing the follower engine.
6. Define a clean selection interface that P5 can later feed with happiness/mood-derived reaction IDs without rewriting the A-button integration.
7. Ensure authored story reactions remain able to take precedence over generic A-button interaction when a scene owns the Partner reaction.
8. Reuse the accepted P3 emote, native movement, final-pose, cry, portrait, and cleanup pipeline unchanged unless a genuine P4 integration defect requires a targeted correction.
9. Keep debug-browser behavior test-only; normal gameplay interaction must not expose the debug manual pre-portrait gate.
10. Do not invent unresolved Yellow mood-selection behavior during P4. P4 establishes interaction dispatch; P5 owns generic mood/happiness selection once source evidence is applied.

After the Partner interaction foundation reaches the appropriate stable boundary, the remaining immediate Cerulean story scope is:

- Melanie / Bulbasaur gift using canonical Partner Pikachu happiness;
- Misty / Cerulean Gym Yellow identity validation;
- Cerulean exit progression toward Vermilion;
- remaining Route 4 / Nugget Bridge / Rocket recruiter regression coverage as needed around the accepted path.

Before changing code for P4:

- resolve the live `prototype/v0.1` branch HEAD;
- inspect the existing follower A-button interaction/action resolver before adding hooks;
- inspect the accepted P3 Partner reaction entry points and canonical Partner resolver;
- use `Yellow-portrait.md` and `pret/pokeyellow` source mappings as behavior authority where P4 requires Yellow semantics;
- preserve accepted P1/P2/P3 rendering, emote, movement, pose, and cleanup behavior;
- avoid duplicating follower identity or reaction-selection state.

---

# 9. Major Yellow Requirements Still Ahead

The following are examples of required Yellow content that remains ahead of the current accepted boundary.

## Partner Pikachu system

- P4 normal A-button Partner interaction path;
- P5 generic Yellow happiness/mood selection behavior;
- P6 one-shot reaction modifiers once source triggers are verified;
- P7 migration/integration of authored reactions onto the common reaction system where appropriate;
- Partner evolution-refusal presentation when applicable;
- later audio fidelity work where Yellow-specific PCM/cry behavior is required and feasible.

## Cerulean era

- Melanie's Bulbasaur gift with canonical Partner happiness requirement;
- Misty Yellow battle identity validation.

## Vermilion era

- S.S. Anne Yellow Rival progression audit;
- required Pokémon Fan Club Partner Pikachu reaction;
- Lt. Surge Yellow battle identity audit;
- Officer Jenny's Squirtle gift after Lt. Surge / Thunder Badge.

## Later mandatory Partner reaction

- Pokémon Tower fear/unease reaction.

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

The accepted Bill behavior, P1/P2 portrait renderer/presentation, and P3 Partner reaction director are regression targets for P4 and later Partner-system work.

---

# 12. Current Blockers

**No known blocking regression is open at this snapshot.**

The full portrait catalog/generalized renderer and reusable P3 reaction-program director have both passed manual acceptance. The final accepted P3 build uses the native follower scripted-movement executor and passed the full 0–32 reaction review.

Development can proceed directly into P4, the normal A-button Partner interaction layer, using the accepted P1/P2/P3 foundation as fixed architecture.

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