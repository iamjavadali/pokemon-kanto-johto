# Pokémon Golden Yellow — Current Project State

**Document type:** Broad operational project snapshot  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Snapshot date:** September 1, 2026

---

# 1. Purpose

This document answers one operational question:

> **Where does Pokémon Golden Yellow broadly stand now, and what is the next active development boundary?**

It is intentionally a high-level snapshot. It is not the permanent evidence ledger for individual gameplay acceptances, workflow runs, ROM hashes, or fix-by-fix history.

Use:

- `Pokemon_Golden_Yellow_Acceptance_Log.md` for cumulative accepted gameplay milestones;
- dedicated acceptance records for complex accepted scenes;
- `Yellow-portrait.md` for the Yellow Partner portrait/reaction source audit and reconstruction blueprint;
- the live `prototype/v0.1` repository for actual implementation state.

---

# 2. Documentation Roles

The Golden Yellow documentation set has distinct responsibilities:

1. `Pokemon_Golden_Yellow_Project_Master.md` — project scope, architecture, campaign direction, and long-term requirements.
2. `Pokemon_Golden_Yellow_Hard_Rules.md` — development governance and non-negotiable engineering rules.
3. `Pokemon_Golden_Yellow_Yellow_Unique_Specification.md` — Yellow-specific Kanto narrative requirements.
4. `Pokemon_Golden_Yellow_FireRed_Retention_Specification.md` — compatible FireRed/Gen III systems and presentation to retain.
5. `Pokemon_Golden_Yellow_Current_Project_State.md` — broad current development position.
6. `Pokemon_Golden_Yellow_Acceptance_Log.md` — canonical cumulative manual-acceptance ledger.
7. `Yellow-portrait.md` — Yellow Partner portrait/reaction source audit and phased reconstruction blueprint.
8. Dedicated acceptance records — deep history for regression-sensitive accepted milestones.

Current Project State must remain broad enough to stay useful between individual fixes.

---

# 3. Current Campaign and Partner-System Position

The initial Yellow-era Kanto campaign remains in active development.

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
→ **P7B Bill authored A-button precedence — ACCEPTED**  
→ **P7C Vermilion Pokémon Fan Club authored integration — ACCEPTED**  
→ **P7D Pokémon Tower fear/unease authored area reaction — ACCEPTED**  
→ **P7E unified Yellow Partner interaction priority / P7 closeout — ACCEPTED**  
→ **P8 Modern follower coexistence — NEXT ACTIVE BOUNDARY**

The P7 authored-reaction phase is now closed as an accepted regression baseline.

The final P7 closeout also validates the corrected exit-ownership behavior for the three scene-owned Partner states that can temporarily prevent room exit:

- Pewter Pokémon Center sleeping Partner;
- Bill's Sea Cottage scene-owned/parked Partner state;
- Vermilion Pokémon Fan Club parked Partner state.

The accepted geometry uses a center-upper guard plus side-warp guards so lateral entry onto a side warp cannot bypass the scene. Bill and Fan Club retain a south-facing-only center-upper check while their side-warp checks are direction-independent when the blocking state is active.

---

# 4. High-Level Campaign Status

| Campaign area | Broad status |
|---|---|
| Yellow opening / Pallet / Oak Lab | Implemented and accepted |
| Viridian / early Route 2 / Viridian Forest | Implemented for current approved scope |
| Pewter / Brock / post-Brock progression | Implemented and accepted for current approved scope; P7A authored Partner lifecycle accepted |
| Route 3 / Mt. Moon | Implemented and accepted, including Jessie & James encounter #1 |
| Cerulean / Route 24 / Route 25 / Bill | Rival/Fame Checker, Charmander rescue, Bill transformed/restored reactions, P7B precedence, and final Bill follower/exit lifecycle accepted; Bulbasaur/Misty remain separate boundaries |
| Vermilion Pokémon Fan Club | Required Yellow Partner scene accepted, including parked/rejoin lifecycle and complete exit guarding |
| Pokémon Tower Partner reaction | Required repeatable Yellow fear/unease direct-talk override accepted on Tower 1F–7F |
| Remaining Vermilion / S.S. Anne / Lt. Surge | Pending staged audit/integration |
| Remaining Lavender / Pokémon Tower story | Pending staged audit/integration, including Rival/Tower story and Jessie & James encounter #3 |
| Celadon / Rocket Hideout | Pending staged audit and Yellow integration |
| Fuchsia / Saffron / Cinnabar / Viridian Gym | Pending staged audit and Yellow integration |
| Victory Road / Indigo Plateau | Pending staged audit and Yellow integration |
| Sevii campaign | Future campaign stage |
| Johto / Crystal campaign | Future campaign stage |
| Later Kanto return / endgame | Future campaign stage |

The accepted authored Partner reaction work does not imply that the surrounding city/region story progression is already complete.

---

# 5. Established Foundation Systems

## 5.1 Canonical Partner Pikachu

Golden Yellow distinguishes:

- canonical Partner Pikachu: `SPECIES_PIKACHU_STARTER`;
- ordinary Pikachu: `SPECIES_PIKACHU`.

Partner-aware systems resolve the canonical Partner specifically rather than treating every Pikachu as the Partner.

Current architecture includes persistent Partner following, Partner-specific restrictions, Partner-aware checkpoint reconstruction, actual follower Pokémon identity resolution, and Partner-only Yellow narrative reactions.

## 5.2 Follower and Scripted Movement Foundation

The project has follower-safe scripted movement for scenes that need Partner Pikachu to remain visible during controlled player/NPC choreography.

P3 established native `ScriptMovement` sequencing as the reusable Partner body-language backend. P7A–P7C proved that authored scenes can temporarily own/stage the existing follower object and return it safely to normal follower authority.

The final P7 closeout additionally proves that scene-owned Partner states must guard all usable exit lanes, including lateral approaches to side warp tiles.

## 5.3 Yellow Rival Eevee State

Persistent Yellow Rival branching exists for:

- Jolteon path;
- Flareon path;
- Vaporeon path.

The branch is established by the Oak Lab and optional early Route 22 battles and is intended to control later Yellow Rival progression.

Later Rival encounters remain separate story-boundary audits.

## 5.4 Trainer Watch / Time Foundation

Trainer Watch and persistent time architecture exist as early foundations for later Crystal-era time-sensitive systems.

## 5.5 Campaign Phase Foundation

Persistent campaign-phase architecture exists for Yellow Kanto → Sevii → era transition → Johto → later Kanto → endgame.

The state architecture exists; later campaigns are not implemented merely because their phase constants exist.

## 5.6 Debug / Checkpoint Foundation

Golden Yellow debug checkpoints reconstruct story state for direct event testing.

A checkpoint is testing infrastructure only and must never be treated as proof that the corresponding event is implemented or accepted.

## 5.7 P1/P2 Portrait Foundation

P1/P2 are accepted.

The established portrait foundation includes:

- directly selectable Yellow portrait programs `0–28`;
- original Yellow 40×40 artwork preserved without scaling;
- native 64×64 GBA OBJ carriers;
- reconstructed multi-stage visible portrait timelines;
- Yellow-derived timing translated to GBA timing;
- script blocking while portraits are active;
- deterministic sprite/window/palette/task cleanup;
- accepted centered, tightened portrait presentation and Pikachu-themed frame treatment.

## 5.8 P3 Reaction-Program Director

P3 is accepted.

The common reaction director supports Yellow Emotion IDs `0–32` through typed reaction commands for cry, emote, movement, portrait, delay, turn-away, and semantic callbacks.

It preserves canonical Partner-only gating and reuses the accepted portrait renderer and follower movement infrastructure.

## 5.9 P4/P5/P6 Implemented Layers

The live implementation includes:

- P4 canonical Partner A-button routing through the common director;
- P5 Yellow-style friendship/mood selection and persistent mood state;
- P6 supported one-shot Yellow reaction modifiers.

These layers remain **IMPLEMENTED** unless separately accepted by explicit manual scope. Their inclusion in the accepted P7 routing baseline does not retroactively convert every P4/P5/P6 behavior into an independently tested acceptance.

## 5.10 P7 Accepted Authored-Reaction Foundation

P7A–P7E are accepted as the authored Yellow precedence layer.

The authoritative field-interaction priority is now:

**Authored story ownership**  
→ **actual battle/status reaction**  
→ **authored area reaction**  
→ **P6 one-shot modifier**  
→ **P5 friendship/mood fallback**

Accepted authored ownership includes:

- P7A Pewter sleep/wake lifecycle;
- P7B Bill manual pre-/post-restoration reactions;
- P7C Fan Club automatic Emotion `29`, parked state, manual Emotion `30`, and follower rejoin;
- P7D repeatable Pokémon Tower Emotion `22` on 1F–7F;
- P7E single-owner routing so lower-priority systems do not consume or override higher-priority Yellow reactions.

The final accepted P7 regression build also includes the stabilized Bill follower lifecycle and the corrected side-warp exit guards for Bill and Fan Club.

---

# 6. Established Design Decisions

- Pokémon Yellow owns the initial Kanto narrative identity.
- FireRed/LeafGreen owns compatible GBA map architecture, presentation, facilities, and proven choreography where Yellow is silent.
- `pokeemerald-expansion` provides the modern systems baseline where compatible.
- Pokémon Crystal owns the later Johto narrative identity.
- TMs remain consumable; reusable TMs are not enabled.
- Modern battle systems remain active, including Natures, Abilities, IVs/EVs, held items, modern movesets, and the Physical/Special split.
- Compatible FireRed quality-of-life systems are retained unless they conflict with Yellow canon.
- Gen I and Gen II Pokémon must ultimately be obtainable in a self-contained single-player save.
- Trade-dependent evolutions must ultimately have single-player alternatives.
- Accepted gameplay should not be reopened without a regression or explicitly approved redesign.
- P4 and later Partner work must reuse the P1/P2/P3 foundation rather than create parallel reaction systems.
- P7 semantic precedence is single-owner: higher-priority story/status/area states cannot be silently overridden by one-shot or normal mood selection.
- Scene-owned Partner exit prevention must protect every traversable exit lane; a guard placed on a side warp must not be defeated merely by entering it laterally.

---

# 7. Accepted Story and Engine Boundary

The current accepted regression baseline reaches through:

- Yellow opening and Partner foundation;
- Viridian Teachy TV integration;
- Pewter/Brock early progression;
- Pewter Jigglypuff reaction and P7A lifecycle;
- Route 3 / Mt. Moon fossil progression;
- Jessie & James encounter #1;
- battle follower send-out identity correction;
- Cerulean Rival #3 / Fame Checker / Partner choreography;
- Route 24 Charmander rescue / treatment / Damian adoption;
- Sea Cottage Bill transformation and Yellow portrait reactions;
- P7B Bill manual A-button precedence;
- P7C Vermilion Fan Club authored Partner scene and rejoin lifecycle;
- P7D Pokémon Tower fear/unease direct-talk override on 1F–7F;
- P7E unified Partner interaction priority;
- final accepted Pewter/Bill/Fan Club exit-guard coverage and Bill follower release lifecycle;
- P1/P2 portrait catalog/renderer and P3 reaction director.

Detailed commits, workflow runs, ROM hashes, and manual-validation notes belong in the Acceptance Log and dedicated acceptance records.

---

# 8. Current Active Development Boundary — P8 Modern Follower Coexistence

P7 is accepted. The next Partner-system boundary is **P8 — Modern follower coexistence** from the `Yellow-portrait.md` reconstruction plan.

P8 must layer the existing modern contextual follower behavior underneath the accepted Yellow ownership/priority system without creating contradictory or duplicate reactions.

The next pass should begin by resolving live `prototype/v0.1` HEAD and inspecting the exact current follower interaction/message path before changing code.

P8 objectives are:

1. preserve the accepted P7 priority resolver as authoritative for Yellow-owned Partner reactions;
2. preserve ordinary follower interaction behavior unchanged;
3. ensure canonical Partner Pikachu can still receive compatible modern contextual follower behavior when no Yellow-owned reaction forbids it;
4. prevent modern contextual text/choreography from replacing or visually colliding with authored Yellow story/status/area reactions;
5. avoid a second Partner interaction engine;
6. keep `SPECIES_PIKACHU_STARTER` as the only canonical Partner identity;
7. protect P7A–P7E and final exit/follower lifecycle behavior as regression baselines;
8. require manual gameplay validation before P8 is accepted.

P9 Yellow audio fidelity remains a later stage after P8.

---

# 9. Major Yellow Campaign Requirements Still Ahead

## Partner system

- P8 modern follower coexistence;
- P9 audio fidelity if/when the project proceeds with original Yellow Pikachu PCM coverage;
- any remaining Partner evolution-refusal presentation or one-shot integration not already completed under P6.

## Cerulean era

- Melanie's Bulbasaur gift with canonical Partner happiness requirement;
- Misty Yellow battle identity validation.

## Vermilion era

- S.S. Anne Yellow Rival progression audit;
- Lt. Surge Yellow solo-Raichu identity audit;
- Officer Jenny Squirtle gift after Lt. Surge / Thunder Badge.

## Lavender / Pokémon Tower

- Rival Battle #5 Yellow progression;
- Jessie & James Pokémon Tower encounter;
- remaining Tower story integration beyond the already accepted Partner fear reaction.

## Remaining Jessie & James encounters

1. Rocket Hideout B4F;
2. Pokémon Tower 7F;
3. Silph Co. 11F.

Additional Yellow-specific trainer, boss, legendary, gift, and narrative differences remain to be staged through the rest of Kanto.

---

# 10. FireRed / Modern Retention Direction

Compatible retained systems continue to include the approved direction for:

- Running Shoes;
- Bicycle and Fly infrastructure;
- Fame Checker;
- Quest Log / resume behavior;
- Vs. Seeker;
- modern Bag organization;
- consumable TMs with modern TM UI;
- Berries;
- Move Tutors and Move Reminder;
- modern Pokédex and PC systems;
- Pokémon Center and Mart systems;
- Sevii Islands and Celio progression;
- Sevii Rocket storyline;
- breeding facilities;
- League rematches.

Inherited engine presence is not the same as Golden Yellow-specific validation. Audit each system when it becomes relevant to active story work.

---

# 11. Acceptance and Regression Policy

Manual gameplay acceptance is tracked in `Pokemon_Golden_Yellow_Acceptance_Log.md`.

Rules:

- build success is necessary but not sufficient for acceptance;
- explicit user gameplay validation controls final acceptance;
- accepted behavior remains a regression baseline unless a defect is found or redesign is approved;
- Current Project State stays broad and does not duplicate detailed evidence.

The accepted P1/P2/P3 foundation and P7 authored-reaction phase are protected regression targets for P8.

---

# 12. Current Blockers

**No confirmed blocking regression is open at this snapshot.**

The final P7 exit-guard/lifecycle correction passed manual testing, and the project is clear to begin the P8 audit/implementation boundary.

---

# 13. Maintenance Rule

Acceptance closeout is governed by Hard Rule `DOC-006`.

Update this document only when the broad project boundary changes, such as:

- a major story region moves from pending to active;
- a major foundation system is added or replaced;
- a blocker materially changes project direction;
- the active Partner-system phase advances;
- the campaign enters a new phase.

For individual accepted fixes and exact evidence, update `Pokemon_Golden_Yellow_Acceptance_Log.md` instead.
