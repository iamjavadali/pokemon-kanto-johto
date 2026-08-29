# Pokémon Golden Yellow — Current Project State

**Document type:** Broad operational project snapshot  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Snapshot date:** August 29, 2026

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
7. Dedicated acceptance records — deep implementation history for major milestones when warranted.

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
→ **Partner Pikachu graphics/reaction-system reconstruction — ACTIVE ENGINEERING BOUNDARY**

The accepted gameplay baseline now extends through the Yellow Cerulean Rival #3 / Fame Checker integration, Route 24 Charmander rescue / Damian adoption milestone, and the required Bill / Sea Cottage Partner Pikachu transformed/restored reaction.

The Bill milestone also validates the first original-Yellow Pikachu portrait vertical slice on native GBA architecture.

The immediate next engineering boundary is to import Yellow's complete Pikachu portrait/graphics catalog and reconstruct the reusable Yellow Partner reaction/emotion system while preserving the accepted Bill behavior as the regression baseline.

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

## 5.7 Yellow Pikachu Portrait Vertical Slice

The accepted Bill milestone establishes a proven narrow GBA rendering path for original Pokémon Yellow Partner Pikachu portrait artwork.

The validated slice includes:

- original Yellow 40×40 portrait art carried inside a native 64×64 GBA OBJ;
- Yellow-derived frame timing translated to GBA timing;
- script blocking while a portrait is active;
- A/B or timer dismissal;
- deterministic portrait resource cleanup;
- centered portrait presentation;
- canonical Partner-only gating.

This is **not yet the complete Yellow Pikachu emotion system**. It is the accepted technical foundation that the next pass should generalize into the full reusable system.

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

For the exact content of accepted milestones, use the Acceptance Log.

---

# 7. Accepted Story Boundary

The currently accepted Yellow story boundary reaches through:

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
- centered original-Yellow confused/questioning and shocked/surprised portrait presentation for those Bill reactions.

The details, commits, workflow runs, ROM hashes, and manual-validation notes for these milestones are maintained in `Pokemon_Golden_Yellow_Acceptance_Log.md` and any linked dedicated acceptance record.

---

# 8. Current Active Development Boundary — Full Yellow Partner Pikachu Reaction System

The Cerulean-era story pass remains active, but the immediate next work is an engine/content foundation pass before proceeding deeper into the story.

The active engineering boundary is:

**import complete Yellow Pikachu graphics → catalog/map Yellow portrait animations → reconstruct reusable GBA portrait/reaction engine → preserve accepted Bill behavior → integrate remaining required Yellow Partner reactions**

The next pass should build outward from the accepted Bill vertical slice rather than creating another one-off scene implementation.

Expected system scope includes:

1. Import the complete usable Yellow Pikachu portrait/graphics catalog with source provenance.
2. Preserve Yellow's semantic mapping between portrait animation scripts and reaction contexts.
3. Build a reusable resource registry rather than hard-coding Bill-only assets.
4. Generalize the native GBA portrait renderer while retaining physical-GBA-safe resource handling.
5. Reconstruct Yellow's reaction-director behavior where appropriate: portrait, cry, emote, movement, delay, and scene-specific commands.
6. Recreate the generic Yellow happiness/mood portrait selection behavior without treating ordinary Pikachu as the Partner.
7. Integrate or migrate the already-required authored reactions so they share the common system where practical.
8. Keep the accepted Bill transformed/restored choreography and portrait result unchanged during generalization.

After that system foundation is stable, the remaining immediate Cerulean story scope is:

- Melanie / Bulbasaur gift using canonical Partner Pikachu happiness;
- Misty / Cerulean Gym Yellow identity validation;
- Cerulean exit progression toward Vermilion;
- remaining Route 4 / Nugget Bridge / Rocket recruiter regression coverage as needed around the accepted path.

Before changing code in the new system pass:

- resolve the live branch HEAD;
- inspect the complete current Bill renderer and all existing Partner/follower helpers;
- use `pret/pokeyellow` as the source authority for portrait assets, emotion definitions, animation scripts, and event mappings;
- translate behavior to GBA architecture rather than porting Game Boy tile-engine assumptions literally;
- retain canonical `SPECIES_PIKACHU_STARTER` gating;
- design the system so later Vermilion Fan Club and Pokémon Tower reactions do not require duplicate renderers.

---

# 9. Major Yellow Requirements Still Ahead

The following are examples of required Yellow content that remains ahead of the current accepted boundary.

## Partner Pikachu system

- complete Yellow Pikachu portrait/graphics import;
- reusable Yellow reaction/emotion architecture;
- generic happiness/mood portrait behavior;
- Partner evolution-refusal presentation when applicable;
- migration/integration of authored reactions onto the common system where appropriate.

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

The accepted Bill behavior must remain a regression target while the portrait/reaction renderer is generalized.

---

# 12. Current Blockers

**No known blocking regression is open at this snapshot.**

The Bill / Sea Cottage Partner Pikachu portrait milestone has passed manual acceptance after the portrait-centering correction and successful prototype build.

Development can proceed from the accepted Bill baseline into the complete Yellow Pikachu graphics/reaction-system reconstruction.

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
