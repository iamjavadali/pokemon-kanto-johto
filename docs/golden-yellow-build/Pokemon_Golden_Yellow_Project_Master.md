# Pokémon Golden Yellow — Project Master Document

**Document type:** Living project master specification  
**Project:** Pokémon Golden Yellow  
**Target platform:** Game Boy Advance  
**Primary engine direction:** FireRed-compatible modern GBA decompilation architecture  
**Current scope:** Full Pokémon Yellow Kanto campaign → FireRed Sevii bridge → three-year transition → Pokémon Crystal Johto campaign → later Kanto return → Mt. Silver/endgame  
**Pokédex target:** #001–#251, fully obtainable in a single ROM/save file  
**Status:** Active development specification  
**Last consolidated:** August 26, 2026

---

# 1. Purpose of This Document

This document is the authoritative living specification for Pokémon Golden Yellow.

It defines:

- the game’s final intended identity;
- narrative canon and source hierarchy;
- engine and systems architecture;
- gameplay modernization rules;
- Partner Pikachu behavior;
- region and campaign progression;
- encounter and Pokédex philosophy;
- cross-region state management;
- Generation III and selected later-generation mechanics;
- quality-of-life systems;
- endgame architecture;
- development stages;
- implementation status terminology;
- validation expectations;
- hardware and emulator constraints;
- scope boundaries intended to prevent feature drift.

This document is not a historical changelog. Overridden project directions are intentionally excluded.

When future implementation plans conflict with this document, the conflict must be explicitly resolved before development proceeds.

---

# 2. Project Identity

Pokémon Golden Yellow is a custom Game Boy Advance Pokémon project that merges the core narrative progression of Pokémon Yellow and Pokémon Crystal into one continuous game built on Gen III GBA architecture.

The project is not intended to be:

- a literal Game Boy or Game Boy Color port;
- a collection of disconnected ROM hacks;
- a FireRed story with Yellow references;
- a Crystal remake with an abbreviated Kanto introduction;
- a multi-ROM experience.

It is intended to be one persistent Pokémon adventure that begins as a faithful modernized Pokémon Yellow remake and later evolves into a Crystal-led Johto campaign within the same save file and game engine.

The long-form campaign is:

**Pokémon Yellow Kanto**  
→ **FireRed Sevii Islands**  
→ **approximately three-year transition**  
→ **Pokémon Crystal Johto**  
→ **S.S. Aqua / post-League transition**  
→ **return to evolved Kanto**  
→ **16-badge endgame**  
→ **Mt. Silver / final postgame**

---

# 3. Core Design Pillars

## 3.1 Yellow Story Fidelity

The first Kanto campaign must preserve Pokémon Yellow’s unique event structure rather than falling back to standard FireRed story logic.

This includes, among other Yellow-specific content:

- Oak intercepting the player when leaving Pallet Town;
- the special wild Pikachu opening event;
- Partner Pikachu becoming the player’s canonical starter;
- the rival receiving Eevee;
- Yellow-specific Bulbasaur, Charmander, and Squirtle gifts;
- Jessie, James, and Meowth appearances;
- Yellow-specific Team Rocket encounters;
- Yellow-specific legendary placement where applicable;
- Yellow progression and event ordering.

FireRed can supply maps, engine infrastructure, UI, and presentation, but it must not silently replace Yellow story canon.

## 3.2 Gen III GBA Presentation

The final game should look and feel like a native GBA Pokémon title.

FireRed/LeafGreen provide the baseline for:

- overworld rendering;
- map scale and tile standards;
- menu structure;
- battle scene presentation;
- trainer presentation;
- GBA-style UI;
- sprites and palettes;
- GBA input expectations;
- save architecture.

Temporary Yellow/GBC assets may be used during development where useful for logic validation, but final presentation should move toward FireRed/LeafGreen-quality GBA assets.

## 3.3 Modernized Mechanics

The game should preserve classic narrative identity without reproducing obsolete battle limitations.

The engine should support:

- Abilities;
- Natures;
- modern IVs and EVs;
- updated movesets;
- held items;
- double battles;
- weather;
- Physical/Special split;
- modernized move behavior where appropriate;
- improved move teaching;
- consumable TMs with modern organization;
- improved trainer rematches;
- richer postgame systems.

## 3.4 Single-Player Completeness

Every Pokémon from #001 through #251 must be obtainable within the game without another cartridge or external event distribution.

The game must remove:

- version exclusives;
- mandatory trade evolutions;
- event-only distribution barriers;
- requirements for a second game or player.

## 3.5 Cross-Region Continuity

The project must behave as one game.

Story state, Partner Pikachu identity, captured Pokémon, major event flags, progression state, and postgame unlocks must persist cleanly across Kanto, Sevii, Johto, and the later Kanto return.

## 3.6 Physical Hardware Compatibility

The ROM must remain suitable for:

- original Game Boy Advance hardware;
- common flash carts where compatible;
- major GBA emulators.

RAM, VRAM, save data, asset pointers, map data, scripts, and runtime systems must remain within real hardware constraints.

---

# 4. Canon and Source Hierarchy

When source games conflict, use the following hierarchy.

| Source | Primary authority |
|---|---|
| Pokémon Yellow | Kanto story, Yellow-specific events, Yellow encounter/event canon |
| Pokémon FireRed/LeafGreen | GBA presentation, map foundation, UI, engine structure, Sevii Islands |
| Pokémon Crystal | Johto story and primary Generation II narrative canon |
| Pokémon Gold/Silver | Supplemental Johto/version differences where Crystal does not define behavior |
| Pokémon Emerald | Reusable Gen III systems, battle facilities, postgame architecture, technical reference |
| `pokeemerald-expansion` | Modern mechanics/system implementation authority where compatible with project canon and FireRed target architecture |

## 4.1 Canon Conflict Rule

During the Yellow campaign:

**Yellow narrative canon overrides FireRed narrative canon.**

Example:

- Moltres remains in Victory Road because that is the Yellow-era canonical placement selected for this project.
- FireRed’s Mt. Ember Moltres placement must not overwrite that decision merely because FireRed maps already support it.

## 4.2 System vs. Story Rule

A system may be borrowed from FireRed or Emerald without importing the source game’s story context.

Examples:

- Battle Frontier concepts may be adapted without importing Hoenn’s Battle Frontier literally.
- FireRed maps may be used while scripts are rewritten to follow Yellow.
- Emerald battle infrastructure may power Golden Yellow-specific postgame content.

## 4.3 Trainer Parity Rule

Golden Yellow does not require literal trainer-by-trainer Yellow parity when FireRed already preserves the trainer's major story role and roster identity.

For non-exclusive trainer battles:

- retain proven FRLG trainer structure when it remains semantically faithful;
- do not change levels, moves, or AI solely to reproduce a Yellow number-for-number match;
- preserve Yellow-specific identity when the roster or narrative role materially differs.

Current approved early-Kanto example: Brock's FRLG Geodude + Onix structure is sufficient to preserve his Yellow story identity; no Yellow-only level/move rewrite is required solely for parity.

---

# 5. Campaign Architecture

# 5.1 Phase I — Pokémon Yellow Kanto

The game begins exclusively as a modernized remake of Pokémon Yellow.

Primary objectives:

- reproduce Yellow’s complete Kanto story;
- establish Partner Pikachu;
- preserve rival Eevee;
- preserve Yellow starter gifts;
- preserve Jessie/James content;
- preserve Yellow progression;
- modernize the battle engine without rewriting narrative identity.

The player earns the original eight Kanto badges and completes the Kanto Pokémon League.

# 5.2 Phase II — Sevii Islands Bridge

Following completion of the Yellow Kanto campaign, the game continues into FireRed-derived Sevii Islands content.

The Sevii sequence serves several purposes:

- gives the player a substantial post-Kanto adventure;
- uses valuable existing GBA content;
- extends the Champion-era Kanto timeline;
- creates narrative space before Johto;
- foreshadows changes that will matter later.

The Sevii phase remains intentionally Gen I-focused.

Generation II species should not be widely introduced here.

# 5.3 Phase III — Three-Year Transition

The game then advances approximately three years toward the Crystal-era timeline.

This transition must be represented deliberately in game state rather than treated as a cosmetic title card only.

Systems affected may include:

- NPC states;
- world flags;
- region access;
- trainer teams;
- certain map events;
- story progression;
- player party restrictions;
- storage access;
- rival/world evolution;
- regional encounter availability.

Partner Pikachu remains the player’s continuity anchor.

# 5.4 Phase IV — Pokémon Crystal Johto

The Johto campaign should primarily follow Pokémon Crystal’s story.

Important principles:

- Johto is the meaningful debut of Generation II species;
- Crystal is the narrative authority;
- day/night and time-based systems must matter;
- the region should feel temporally and mechanically more advanced than early Kanto;
- the same player/save continues from the Kanto era.

# 5.5 Phase V — Return to Kanto

After Johto League progression and the S.S. Aqua transition, the player returns to a later version of Kanto.

This Kanto should not simply be the unchanged first-campaign map state.

It should reflect approximately three years of world progression.

Potential state differences include:

- updated trainer teams;
- changed NPC dialogue;
- altered access;
- evolved world events;
- changed facilities;
- revised wild encounters where canonically appropriate;
- stronger rematches;
- later-game rewards;
- continuity callbacks.

The player ultimately completes the full 16-badge structure.

# 5.6 Phase VI — Mt. Silver / Final Endgame

Mt. Silver is positioned as a late or final endgame destination.

This phase should represent the culmination of:

- two regional campaigns;
- 16 badges;
- Champion progression;
- Partner Pikachu continuity;
- high-level team development;
- postgame systems.

---

# 6. Partner Pikachu Architecture

Partner Pikachu is a special persistent Pokémon and must not be treated as an ordinary Pikachu with a single evolution lock.

## 6.1 Required Partner Pikachu Behavior

Partner Pikachu must:

- be obtained through the Yellow opening sequence;
- remain the player’s canonical partner;
- follow the player;
- be distinct from ordinary Pikachu;
- remain unevolved;
- not be tradable away;
- survive major campaign transitions;
- retain identity across Kanto, Sevii, Johto, and later Kanto.

## 6.2 Ordinary Pikachu

Ordinary Pikachu must remain normal members of the species.

They may:

- be encountered;
- be captured;
- use ordinary species data;
- evolve into Raichu.

The project must never globally disable Pikachu evolution merely to protect Partner Pikachu.

## 6.3 Existing Engine Identity

The FireRed-compatible source already contains a starter-specific species identity:

`SPECIES_PIKACHU_STARTER`

Where technically sound, this existing identity should be reused rather than creating unnecessary duplicate save or species systems.

The architecture should remain modular enough that Partner-specific logic can be implemented through:

- species identity;
- persistent flags;
- party checks;
- save-state metadata;
- dedicated helper functions.

Avoid scattering special-case Pikachu logic through unrelated map scripts.

## 6.4 Canonical Partner Identity Rule

Partner-only behavior must resolve the canonical Partner identity, not merely test for the ordinary Pikachu species.

Current canonical identity:

- Partner Pikachu: `SPECIES_PIKACHU_STARTER`
- ordinary Pikachu: `SPECIES_PIKACHU`

A generic species-only Pikachu check must never cause ordinary Pikachu to inherit Partner-only restrictions or narrative reactions.

## 6.5 Required Yellow Partner Reactions

The Yellow campaign must include, at minimum, the following Partner-specific reaction scenes:

1. Pewter Pokémon Center Jigglypuff sleep/wake reaction.
2. Bill transformed/restored reaction sequence.
3. Vermilion Pokémon Fan Club reaction.
4. Pokémon Tower fear reaction.

These reactions are narrative Partner events and should use reusable follower/emote infrastructure where practical rather than one-off fake Pikachu objects.

---

# 7. Follow Pokémon System

Partner Pikachu is the mandatory canonical follower.

The follower system should be implemented as a reusable engine subsystem rather than a one-map scripted illusion.

Design goals:

- persistent following;
- map transition handling;
- collision safety;
- warp handling;
- stairs/doors/caves;
- bike or movement-mode compatibility;
- cutscene suspension and restoration;
- party-state awareness;
- future support for other eligible party Pokémon.

Partner Pikachu remains the first required production use case.

---

# 8. Starter and Gift Pokémon Rules

## 8.1 Partner Pikachu

Partner Pikachu replaces the normal three-choice starter selection.

## 8.2 Rival

The rival receives Eevee.

Rival progression should preserve Yellow’s Eevee identity and appropriately modernize team development.

## 8.3 Bulbasaur

Bulbasaur is obtained through the Yellow story gift.

It remains a normal Bulbasaur and evolves normally.

## 8.4 Charmander

Charmander is obtained through the Yellow story gift.

It remains a normal Charmander and evolves normally.

## 8.5 Squirtle

Squirtle is obtained through the Yellow story gift.

It remains a normal Squirtle and evolves normally.

---

# 9. Jessie, James, and Meowth

Jessie, James, and Meowth are required Yellow-specific characters.

Their encounters should:

- replace generic FireRed Rocket equivalents where Yellow canon requires them;
- retain their anime-inspired identity;
- use GBA-quality presentation;
- preserve their role in Yellow’s progression.

## 9.1 Double Battle Opportunity

Golden Yellow may modernize suitable Jessie and James encounters into double battles where doing so preserves or strengthens the intended scene.

This should be used deliberately rather than rewriting every original encounter structure.

---

# 10. Core Battle Mechanics

The baseline battle engine should use Gen III architecture plus selected later-generation corrections.

Locked core mechanics include:

- Abilities;
- Natures;
- Gen III-style IVs;
- Gen III-style EVs;
- held items;
- status mechanics;
- double battles;
- weather;
- improved movesets;
- modernized move behavior where appropriate;
- Physical/Special split.

## 10.1 Physical/Special Split

The Physical/Special split is intentionally included even though FireRed and Emerald themselves still used type-based damage categories.

Golden Yellow should assign damage category per move.

This is important for:

- more accurate Pokémon role design;
- better Gen II Pokémon viability;
- improved movesets;
- modern battle expectations.

---

# 11. Movement and Overworld Quality of Life

## 11.1 Running Shoes

Running Shoes or equivalent fast-running functionality are locked into project scope.

Goals:

- provide faster movement early;
- reduce unnecessary walking friction;
- preserve opening pacing;
- work throughout appropriate indoor and outdoor maps unless specific map logic requires restriction.

This system should be integrated cleanly rather than implemented through map-specific speed hacks.

---

# 12. Trainer Rematches and Vs. Seeker

A FireRed-inspired Vs. Seeker/rematch system is locked into the project.

However, Golden Yellow should improve the concept through scalable trainer progression.

Suggested trainer progression tiers:

- **Tier 1:** original story encounter;
- **Tier 2:** later Kanto progression;
- **Tier 3:** post-Kanto League / Sevii;
- **Tier 4:** post-Johto / three-years-later Kanto;
- **optional endgame tier:** high-level rematch for selected trainers.

Benefits:

- renewable EXP;
- renewable money;
- world continuity;
- replayable routes;
- visible passage of time;
- better long-game level curve.

Trainer rematches should not simply repeat the original team forever.

---

# 13. Elite Four and Champion Rematches

Stronger League rematches are locked into scope.

Possible progression:

## First Kanto League

Yellow-inspired teams using modern mechanics.

## Later Kanto Rematch

Stronger teams after significant post-League progression.

## Three-Years-Later / Post-Johto Rematch

Substantially upgraded teams that reflect elapsed time and the player’s later-game power level.

Rematches may include:

- evolved Pokémon;
- improved movesets;
- held items;
- Abilities;
- better AI;
- higher levels;
- revised team composition.

---

# 14. Reusable Move Tutors

Move Tutors should be reusable rather than permanently consumed after one teaching action.

Possible costs:

- money;
- region-specific items;
- rare shards or equivalents;
- Battle Points;
- progression unlocks.

Tutors provide a flexible mechanism for modern moveset support across #001–251 without overloading the TM list.

---

# 15. TM Policy

TMs remain **consumable / single-use**.

Golden Yellow intentionally does **not** enable reusable TMs. The project may retain or improve modern TM organization, case/menu presentation, and acquisition flow, but consuming a TM when it teaches a move remains the canonical rule unless the user explicitly changes this project direction later.

Move Tutors are handled separately and may be reusable.

HMs or field-move architecture may be handled separately depending on later implementation decisions.

---

# 16. Enhanced Pokédex

The Pokédex must support the project’s single-player completion goal.

It should become a practical in-game discovery tool rather than merely a list of species.

Target capabilities include:

- Seen status;
- Caught status;
- regional identification;
- habitat/encounter information;
- evolution hints;
- time-of-day requirements;
- special-condition hints;
- known location information.

A preferred progression model is:

- unknown species remain largely hidden;
- once a species is Seen, useful acquisition guidance can begin to unlock;
- once Caught, fuller data becomes available.

The system should help players complete #001–251 without requiring an external wiki for basic acquisition information.

---

# 17. Multi-Region Map / Pokégear Architecture

Golden Yellow must support navigation across:

- Kanto;
- Sevii Islands;
- Johto.

The map system should be designed early for multi-region support.

Target capabilities:

- region switching;
- current player position;
- Fly destinations;
- route connections;
- major towns;
- important facilities;
- special region markers where appropriate.

The system may evolve narratively from an early Town Map into a more capable Johto-era Pokégear-style interface.

Avoid building three unrelated map systems.

---

# 18. Time, Day/Night, and RTC Architecture

Crystal-era gameplay requires a reliable clock system.

The project direction supports a fallback hierarchy similar to:

**RTC → saved clock → manual fallback**

Goals:

- work on real hardware;
- work on emulators with RTC;
- remain playable where RTC support is absent or inconsistent;
- persist time safely;
- support day/night encounters;
- support Crystal-style timed events;
- avoid save corruption or progression lockout.

Time architecture must be designed as a global engine system rather than a Johto-only patch added late.

---

# 19. Generation II Systems to Preserve

Johto should retain important Generation II identity through systems such as:

- day/night;
- friendship;
- breeding;
- roaming Pokémon;
- time-based encounters;
- time-based events;
- region-specific evolution methods where appropriate.

These systems should coexist with Gen III battle mechanics.

---

# 20. Encounter Design Philosophy

The requirement that all #001–#251 are obtainable does not mean all species should be placed everywhere.

Encounter design must preserve regional identity and era progression.

## 20.1 Yellow Kanto

Primarily Generation I species.

## 20.2 Sevii Islands

Remain substantially Gen I-era.

Do not casually introduce the full Generation II ecosystem.

## 20.3 Johto

Generation II species make their meaningful debut here.

This preserves the impact of moving into a new generation and region.

## 20.4 Later Kanto

Later Kanto may selectively reflect the changed timeline and completed Johto progression where appropriate.

---

# 21. Full Pokédex Accessibility

Every species #001–#251 must be obtainable within one game.

This includes:

- starters;
- fossils;
- version exclusives from original games;
- trade evolutions;
- roaming Pokémon;
- legendary Pokémon;
- Mythical/event Pokémon included within project scope;
- one-off gifts or alternative acquisition paths where necessary.

No species should require:

- another ROM;
- another save;
- another player;
- link trading;
- external event distribution.

---

# 22. Alternative Trade Evolutions

Trade-only evolution requirements must be replaced with single-player methods.

Possible mechanisms include:

- level thresholds;
- use-item evolution;
- held-item + level evolution;
- location-based evolution;
- NPC-assisted evolution;
- special event evolution.

The method may vary by species.

The governing rule is:

**No #001–#251 Pokédex completion path may depend on a trade partner.**

---

# 23. Legendary Pokémon

Yellow-era legendary placement should follow Yellow canon where the project has explicitly selected it.

Locked example:

- Moltres remains in Victory Road.

Kanto legendary availability should include:

- Articuno;
- Zapdos;
- Moltres;
- Mewtwo.

Mew must also become obtainable through an internal single-player method rather than an external distribution.

Johto legendary and roaming behavior should be designed around Crystal’s narrative authority.

---

# 24. Roaming Pokémon

The engine should support persistent roaming Pokémon state for Johto-era species such as:

- Raikou;
- Entei;
- Suicune where appropriate.

Crystal’s Suicune narrative treatment should remain distinct from generic roaming logic where canon requires it.

Roamer state must survive:

- save/load;
- map transitions;
- region progression;
- encounter failure;
- defeat/escape rules according to final design.

---

# 25. Breeding

Breeding becomes a deeper system in Johto.

Because Golden Yellow includes:

- Natures;
- IVs;
- Abilities;
- held items;
- modern movesets;

breeding can support later-game team optimization.

The project should avoid forcing an advanced breeding meta into early Yellow Kanto if doing so harms pacing.

---

# 26. Weather

Battle weather should be retained and supported by the engine.

Expected systems include:

- rain;
- sun;
- sandstorm;
- hail where appropriate;
- Ability interactions;
- move interactions.

Overworld weather should be used selectively so Kanto does not feel artificially transformed into Hoenn.

Johto may use atmospheric weather more heavily where it strengthens regional identity.

---

# 27. Berries and Renewable Resources

A restrained Gen III-style berry system may be used.

Potential locations include:

- Berry Forest;
- Johto farms;
- selected NPC gardens;
- other contextually appropriate plots.

Berries can provide renewable support for:

- status recovery;
- battle effects;
- held-item strategies;
- optional advanced systems.

---

# 28. Battle Frontier and Battle Points

An Emerald-inspired Battle Frontier is locked into Golden Yellow as late-game content.

It should not simply import Hoenn’s location and story unchanged.

Instead, Golden Yellow should reuse/adapt the underlying facility concepts to create a project-specific endgame challenge system.

Possible facilities may draw from:

- Battle Tower;
- Battle Factory;
- Battle Dome;
- Battle Arena;
- Battle Pike;
- Battle Palace;
- Battle Pyramid.

Exact final facility selection and location may be decided during the postgame implementation phase.

## 28.1 Battle Points

Battle Points are locked as the associated late-game reward economy.

Potential BP rewards include:

- competitive held items;
- vitamins;
- rare evolution items;
- Move Tutor access;
- advanced training resources;
- cosmetic or follower-related rewards where appropriate.

The Frontier should provide meaningful repeatable content after the player has already built strong teams across both regions.

---

# 29. Systems Not Currently Locked Into Core Scope

The following Gen III systems are not part of mandatory project scope:

- full Hoenn-style Pokémon Contests;
- traditional Hoenn Secret Bases.

They may be reconsidered later only if they serve the project naturally and do not distract from Yellow/Crystal identity.

A future customizable player room or trophy space may be considered independently, but it is not currently a required core feature.

---

# 30. World Economy and Renewable Progression

Because Golden Yellow is much longer than a normal Pokémon game, the economy must not become permanently exhausted.

Renewable sources should exist for:

- money;
- EXP;
- berries;
- evolution items;
- held items;
- move teaching;
- late-game competitive resources.

The following systems contribute directly:

- trainer rematches;
- Vs. Seeker;
- League rematches;
- Battle Frontier;
- Battle Points;
- renewable berry systems;
- consumable TMs with modern organization;
- reusable tutors.

---

# 31. GBA Presentation and Asset Workflow

## 31.1 Placeholder Development Assets

Temporary Yellow/GBC assets and layouts may be used when necessary to validate:

- event logic;
- map flow;
- cutscenes;
- battles;
- NPC movement;
- flags;
- progression.

## 31.2 Final Asset Direction

The final standard should move toward FireRed/LeafGreen-quality GBA graphics.

## 31.3 Replaceability Requirement

Code, map scripts, and asset references should be structured so temporary assets can later be replaced without requiring major gameplay rewrites.

Avoid embedding critical logic into graphics-specific hacks.

---

# 32. Map and Script Architecture

All maps should use clean state-driven scripting.

Important practices:

- reserve flags and variables deliberately;
- avoid region-specific ID collisions;
- centralize major story-state helpers;
- distinguish temporary map state from permanent story state;
- avoid hardcoding cross-region assumptions into isolated scripts;
- use reusable engine functions for repeated mechanics;
- keep save persistence in mind from the start.

Future Johto work must not require dismantling Kanto’s state system.

---

# 33. Cross-Region Save and State Management

The game uses one persistent save.

The save architecture must eventually track:

- Kanto story completion;
- Kanto badges;
- Sevii progression;
- three-year transition state;
- Johto story progression;
- Johto badges;
- later Kanto state;
- Partner Pikachu identity;
- legendary states;
- one-time gifts;
- rival state;
- roaming Pokémon;
- Battle Frontier progress;
- Pokédex progress;
- rematch tiers;
- regional access;
- time data;
- persistent unlocks.

Flags and variables must be allocated with the full game in mind.

---

# 34. Johto Transition Party Rule

Partner Pikachu continues with the player into the Crystal-era campaign.

The full endgame Kanto party should not simply arrive in New Bark Town at full strength without restriction.

The transition system must therefore eventually define:

- active party behavior;
- storage restrictions;
- later restoration;
- level/progression balance;
- access timing;
- Partner Pikachu continuity.

The exact final storage/party implementation remains to be designed.

The locked narrative rule is:

**Partner Pikachu crosses the era transition with the player.**

---

# 35. Trainer and Difficulty Progression

Golden Yellow should use a long-form difficulty curve appropriate to a two-region game.

Avoid:

- extreme early difficulty spikes;
- trivial Johto caused by a full endgame Kanto party;
- static trainer levels across a three-year world transition;
- postgame trainers that remain permanently weak.

Difficulty systems may use:

- rematch tiers;
- improved AI;
- better movesets;
- held items;
- evolved teams;
- selective double battles;
- postgame facility rules.

---

# 36. Code Architecture Principles

## 36.1 Clean C Code

Core mechanics should be implemented in modular C code compatible with the selected decomp framework.

## 36.2 Reuse Existing Systems

Prefer extending existing engine systems over creating duplicate infrastructure.

Example:

Use an existing Partner Pikachu identity mechanism where safe instead of creating an unrelated second Pokémon representation.

## 36.3 Separate Engine Logic from Map Scripts

Persistent mechanics should generally be engine-owned.

Examples:

- Partner restrictions;
- follower state;
- rematch tier selection;
- RTC/time handling;
- regional map state;
- trade-evolution alternatives.

Map scripts should invoke systems, not redefine them repeatedly.

## 36.4 Avoid Parallel Systems

Do not create separate Kanto and Johto implementations of the same fundamental subsystem unless truly required.

Use one extensible architecture for:

- maps;
- time;
- followers;
- Pokédex;
- region travel;
- trainer progression;
- save state.

---

# 37. Hardware and Memory Requirements

The ROM must respect real GBA limitations.

Development should continuously consider:

- EWRAM;
- IWRAM;
- VRAM;
- OAM;
- palette memory;
- ROM space;
- save size;
- DMA use;
- sprite limits;
- map object limits;
- decompression buffers;
- audio resource use.

Features must not be considered complete if they work only in permissive emulator conditions and fail on physical hardware.

---

# 38. Emulator Compatibility

Target compatibility should include major GBA emulators used by the community.

Testing should include multiple emulator environments where practical, particularly for:

- RTC;
- save type;
- audio;
- map rendering;
- timing-sensitive behavior;
- follower movement;
- cutscenes.

---

# 39. Development Status Terminology

Future documentation and passes must distinguish these states.

## 39.1 Planned / Locked

The feature or rule is approved project direction but may not yet exist in code.

## 39.2 Implemented

The feature exists in code.

This does not imply it builds or works correctly.

## 39.3 Build-Validated

The relevant source compiles successfully and produces a valid ROM/artifact.

This does not imply gameplay acceptance.

## 39.4 Automated-Test-Validated

Relevant automated tests pass.

This does not replace manual gameplay validation.

## 39.5 Manually Gameplay-Validated

The feature has been manually tested in-game and behaves as expected.

## 39.6 Accepted

The feature or pass has passed the required validation and is approved as the current implementation source of truth.

These terms must not be used interchangeably.

---

# 40. Live Repository State Is Authoritative

This master document intentionally does **not** store a static current commit, branch head, ROM checksum, build artifact ID, or implementation checkpoint as the authoritative development baseline.

Those details change during normal development and would cause this document to become stale.

Before **every planning pass, implementation pass, debugging pass, code review, or continuation of prior work**, the working chat/session must re-check only the effected files in the live GitHub repository and current project files. do not do read full repo files read. do not re-check if not needed or already checked prior and no addtional changes were made.

At minimum, the live-state check should establish, as relevant to the task:

- the current canonical development branch;
- the branch's current HEAD commit;
- recent commits relevant to the area being changed;
- the actual contents of files that will be planned around or modified;
- existing implementation state before proposing new code;
- current CI/check status where relevant;
- current build artifacts and provenance where relevant;
- whether scratch/work branches are aligned or have diverged;
- whether a prior implementation already exists and should be extended rather than recreated.

The project must **resume from the verified live repository state**, not from an old chat summary, an old implementation plan, a static commit recorded in documentation, or an assumption about what was previously completed.

Historical commit hashes, ROM checksums, artifact IDs, and implementation checkpoints may be recorded in build records, release notes, validation reports, or Git history, but they should not be used as the permanent source-of-truth baseline in this master specification.

If a handoff summary conflicts with the live repository, the live repository and directly inspected current files take precedence for implementation state.

This rule does not change project canon or design requirements defined elsewhere in this document.

---

# 41. Stage 1A Foundation Role

The opening Yellow sequence is the protected architecture foundation for the rest of Stage 1.

It establishes systems that will affect the full game:

- Partner Pikachu identity;
- Oak scripting;
- rival state;
- gift/event Pokémon;
- save persistence;
- follower initialization;
- story flags;
- menu/system initialization;
- time initialization;
- special Pokémon restrictions.

Once accepted, Stage 1A behavior should remain protected while later Kanto work scales the same architecture forward.

---

# 42. Stage 1 Completion Scope

Stage 1 is not merely Stage 1A.

Stage 1 means the complete Pokémon Yellow Kanto campaign.

It broadly includes:

## Foundation

- Yellow opening;
- Oak/Pikachu sequence;
- rival Eevee;
- Partner Pikachu;
- follower system;
- save persistence;
- starter restrictions.

## Full Kanto Route Progression

- Pallet Town;
- Viridian;
- Pewter;
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

## Yellow-Specific Story Content

- Bulbasaur gift;
- Charmander gift;
- Squirtle gift;
- Jessie/James/Meowth;
- Yellow Rocket encounters;
- Yellow-specific NPC/event logic;
- Yellow rival progression.

## Legendary Progression

- Articuno;
- Zapdos;
- Moltres in Victory Road;
- Mewtwo;
- internal Mew acquisition.

## League

- Elite Four;
- Champion;
- first Hall of Fame;
- post-Champion transition.

---

# 43. Stage 1 Completion Gate

Stage 1 should not be declared complete merely because credits can be reached.

A production-grade Stage 1 completion gate should include:

1. Complete Yellow Kanto narrative.
2. Correct Yellow event ordering.
3. Partner Pikachu persists correctly.
4. Partner Pikachu cannot evolve or be lost improperly.
5. Ordinary Pikachu can still evolve.
6. Bulbasaur/Charmander/Squirtle gifts work.
7. Jessie/James/Meowth content works.
8. Rival/Eevee progression works.
9. Yellow legendary placement works.
10. Modern battle systems work across Kanto.
11. Save/reload is reliable.
12. Day/time initialization is safe.
13. Follower behavior is stable.
14. Map transitions are stable.
15. Story flags cannot soft-lock progression.
16. FireRed-quality presentation is sufficiently complete for the phase.
17. Major emulator validation passes.
18. Hardware safety has been reviewed.
19. Full manual New Game → Champion regression passes.
20. Post-Champion handoff into the Sevii phase is deterministic.

---

# 44. Proposed Master Development Roadmap

## Stage 0 — Engine and Repository Foundation

Objectives:

- confirm canonical source branch and commit;
- confirm build chain;
- confirm target decomp framework;
- confirm save type;
- confirm ROM output;
- confirm CI;
- establish coding standards;
- reserve global flag/variable strategy.

## Stage 1 — Complete Yellow Kanto

Objectives:

- all Yellow narrative content;
- Partner Pikachu;
- follower;
- modern battle engine;
- Yellow gifts;
- Jessie/James;
- full League;
- Kanto legendary logic.

## Stage 2 — Sevii Bridge

Objectives:

- adapt Sevii content;
- preserve Gen I era;
- establish post-Champion state;
- begin narrative transition toward Johto;
- validate cross-region transportation.

## Stage 3 — Era Transition Engine

Objectives:

- approximately three-year transition;
- world-state versioning;
- party/storage transition;
- Partner Pikachu continuity;
- unlock Johto;
- prepare later Kanto state.

## Stage 4 — Crystal Johto

Objectives:

- full Crystal-led story;
- Gen II ecosystem;
- day/night;
- breeding;
- roaming Pokémon;
- Johto gyms;
- Johto League;
- Crystal-specific narrative content.

## Stage 5 — Later Kanto

Objectives:

- return via S.S. Aqua;
- evolved Kanto;
- later trainer teams;
- rematch progression;
- remaining badges;
- late-game encounters;
- continuity payoff.

## Stage 6 — Mt. Silver and Final Endgame

Objectives:

- final major challenge;
- 16-badge completion;
- highest-level trainer content;
- full cross-region story closure.

## Stage 7 — Battle Frontier and Completion Systems

Objectives:

- Emerald-inspired Frontier;
- Battle Points;
- competitive held items;
- advanced Move Tutors;
- repeatable endgame;
- Pokédex-completion support;
- final reward loops.

## Stage 8 — Final Graphics and Polish

Objectives:

- replace remaining placeholders;
- consistent FireRed-quality map presentation;
- sprite cleanup;
- UI polish;
- audio polish;
- performance review;
- final hardware regression.

---

# 45. Multi-Region Navigation Requirements

The navigation architecture must support future region expansion without rewriting the underlying map interface.

Suggested region model:

- `REGION_KANTO`
- `REGION_SEVII`
- `REGION_JOHTO`

The exact constants may differ based on codebase conventions.

Systems that should understand region identity:

- Town Map/Pokégear;
- Fly;
- encounter guidance;
- Pokédex locations;
- story travel;
- map metadata;
- certain save-state helpers.

---

# 46. Flag and Variable Strategy

A formal registry should eventually be maintained for:

- global story flags;
- Kanto-only flags;
- Sevii flags;
- era-transition flags;
- Johto flags;
- later-Kanto flags;
- Partner Pikachu flags;
- rematch progression;
- Battle Frontier;
- one-time Pokémon;
- legendary states.

General rules:

- do not casually reuse temporary flags;
- avoid map-local values for global story progression;
- reserve ranges where appropriate;
- document ownership;
- make save migrations deliberate if the save layout changes.

---

# 47. Pokémon Data Strategy

Pokémon #001–#251 should use one coherent data model.

Each species should eventually have reviewed:

- base stats;
- type;
- Ability;
- optional secondary Ability if supported;
- growth rate;
- catch rate;
- gender ratio;
- egg groups;
- hatch data;
- moveset;
- TM compatibility;
- Tutor compatibility;
- evolution method;
- encounter availability.

Partner Pikachu-specific data should remain isolated from ordinary Pikachu where needed.

---

# 48. Moveset Modernization

Movesets should not be copied blindly from Yellow or Crystal.

The objective is to preserve species identity while making Pokémon functional within the modernized engine.

Review criteria:

- Physical/Special split;
- Ability synergy;
- STAB access;
- evolution timing;
- progression balance;
- TM/Tutor availability;
- early-game power;
- late-game viability.

Avoid giving every Pokémon perfect competitive coverage too early.

---

# 49. Evolution Design

Evolution should remain intuitive and single-player compatible.

Use canonical methods where practical.

Replace inaccessible methods when required.

Trade-evolution alternatives must be documented centrally so players can discover them through the enhanced Pokédex or in-world hints.

---

# 50. Level Curve Philosophy

The level curve must account for the unusually long campaign.

Goals:

- early Kanto feels like a traditional starter journey;
- Kanto League feels meaningful;
- Sevii remains challenging after becoming Champion;
- Johto does not become trivial;
- later Kanto scales upward;
- Mt. Silver represents true endgame;
- Frontier content can use dedicated level rules such as Lv. 50 or open-level formats.

The three-year transition may require controlled party access rather than aggressive global level scaling.

---

# 51. AI Philosophy

Trainer AI should improve as progression advances.

Potential progression:

- ordinary early trainers use simple AI;
- Gym Leaders use stronger decision logic;
- League trainers use advanced AI;
- later rematches use held items and better coverage;
- Frontier opponents use facility-specific rules.

Avoid making every early trainer behave like a competitive simulator.

---

# 52. Save Compatibility Philosophy

Because the project is under active development, save-format changes may occasionally be necessary.

However:

- avoid unnecessary save-structure churn;
- document changes;
- keep persistent IDs stable where possible;
- test old-to-new compatibility if claimed;
- require fresh saves where compatibility cannot be guaranteed.

Never imply save compatibility unless it has been explicitly validated.

---

# 53. Validation Requirements for Every Major Pass

Each major implementation pass should define:

## Source

- exact source branch;
- exact starting commit;
- exact resulting commit.

## Build

- clean compile;
- ROM generated;
- ROM size;
- checksum;
- header sanity.

## Automated Validation

Where available:

- unit tests;
- script tests;
- build checks;
- lint/static checks;
- CI.

## Manual Validation

- fresh-save test where relevant;
- save/load;
- map transitions;
- battles;
- event sequencing;
- soft-lock testing;
- menu behavior;
- follower behavior;
- transition behavior.

## Hardware/Emulator Validation

At milestone points:

- major emulator smoke;
- RTC behavior;
- save behavior;
- physical hardware/flash cart test where possible.

---

# 54. ROM Artifact Provenance

Every accepted milestone ROM should be traceable to source.

Recommended milestone metadata:

- ROM filename;
- Git commit;
- branch;
- build timestamp;
- SHA-256;
- ROM size;
- toolchain version;
- validation status;
- notes about fresh-save requirements.

A ROM checksum alone does not establish feature acceptance.

---

# 55. Scope-Control Rules

Before adding any new feature, evaluate:

1. Does it strengthen Yellow, Crystal, cross-region continuity, or endgame quality?
2. Does it fit GBA hardware?
3. Does it increase maintenance burden disproportionately?
4. Can an existing engine system be extended instead?
5. Does it undermine regional identity?
6. Does it create save-format risk?
7. Does it conflict with #001–#251 accessibility?
8. Does it delay core story completion?

Core story and engine stability take priority over novelty.

---

# 56. Locked Generation III / Modernization Additions

The following additions are now explicitly part of the project:

1. Running Shoes / faster overworld movement.
2. Vs. Seeker with scalable trainer rematches.
3. Progressively stronger Elite Four and Champion rematches.
4. Double Battles, including suitable Jessie/James encounters.
5. Reusable Move Tutors.
6. Enhanced #001–#251 Pokédex with encounter/evolution guidance.
7. Multi-region Town Map/Pokégear architecture.
8. Consumable TMs with modern organization and single-use teaching behavior.
9. Emerald-inspired Battle Frontier with Battle Points economy.

These are project requirements, not optional brainstorming items.

---

# 57. Explicitly Deferred / Non-Core Systems

Not currently required:

- Hoenn-style Pokémon Contests;
- traditional Hoenn Secret Bases.

They should not be implemented during core story work unless the project direction is explicitly revised.

---

# 58. Narrative Tone and Continuity

The game should feel like one evolving Pokémon world.

The player should experience:

- childhood adventure in Yellow-era Kanto;
- becoming Champion;
- post-League exploration;
- visible passage of time;
- arrival in a new region and generation;
- return to familiar places that have changed;
- eventual mastery of both regions.

The later Kanto return should reward players who remember the original Yellow-era world.

NPC dialogue, trainer evolution, facilities, and environmental details should reinforce this continuity.

---

# 59. Partner Pikachu as Narrative Continuity Anchor

Partner Pikachu should serve not only as a mechanic but as the strongest emotional link between the Yellow and Crystal eras.

The player may change teams, regions, rivals, gyms, and world states, but Partner Pikachu remains a visible connection to the beginning of the journey.

This is why its identity must be architected correctly from Stage 1 rather than retrofitted during Johto development.

---

# 60. Definition of Final Project Success

Pokémon Golden Yellow is successful when it delivers a stable GBA ROM in which a player can:

- begin in Pallet Town;
- experience the full Yellow story;
- travel with Partner Pikachu;
- obtain the Yellow gift starters;
- encounter Jessie, James, and Meowth;
- complete the original Kanto League;
- explore the Sevii Islands;
- experience the passage into the Crystal era;
- begin the Johto journey without starting a new save;
- encounter Generation II Pokémon meaningfully in Johto;
- complete Crystal-led Johto progression;
- return to an evolved Kanto;
- earn all 16 badges;
- reach Mt. Silver;
- obtain every Pokémon #001–#251;
- complete trade evolutions without trading;
- build advanced teams through modern mechanics;
- use rematches, reusable tutors, consumable TMs, and the enhanced Pokédex;
- participate in a late-game Battle Frontier;
- earn and spend Battle Points;
- continue meaningful play after the main story;
- run reliably on real GBA hardware and major emulators.

---

# 61. Required Planning and Implementation Entry Procedure

Every future planning or implementation session must begin from verified current code, not from remembered implementation status.

Before proposing or applying changes:

1. Re-check the live GitHub repository for only the files affected.
2. Resolve the canonical development branch and its current HEAD.
3. Inspect the current versions of all files directly relevant to the requested work.
4. Review recent relevant commits, diffs, CI results, or artifacts when they affect the task.
5. Determine what is already implemented, partially implemented, validated, or still missing.
6. Continue incrementally from that verified state.
7. Do not recreate an earlier planned fix if equivalent or partial work already exists in the repository.
8. Keep project requirements from this master document separate from implementation-state facts discovered in GitHub.

If the repository cannot be inspected, the session must clearly state that the current implementation state cannot be confirmed and must not present an old static baseline as current.

The purpose of this procedure is to make this master document usable from the beginning of the project through final release without requiring routine updates after each implementation pass.

---

# 62. Master Document Governance and Update Rule

This document is the authoritative specification for **project direction**, not a running implementation log.

It should be updated only when a material project-level decision changes or adds to one or more of the following:

- project scope;
- narrative canon;
- source hierarchy;
- gameplay rules;
- engine architecture rules;
- cross-region architecture;
- hardware/compatibility requirements;
- required systems or mechanics;
- development-stage definitions;
- validation policy;
- scope-control rules;
- other major design or governance decisions.

Routine development progress must **not** require an update to this document.

Examples that should normally **not** trigger a master-document update:

- a new commit;
- a branch advancing;
- a bug fix;
- a ROM checksum changing;
- a new CI run;
- a build artifact ID changing;
- one planned item becoming implemented;
- one implemented item becoming validated;
- ordinary refactors;
- incremental map/script work;
- routine Stage 1, Stage 2, or later implementation progress.

Those facts belong in Git history, PRs, issues, milestone notes, validation reports, build records, or live repository inspection.

Every future planning or implementation pass must use the project documentation and repository according to their distinct roles:

1. **Project Master, Hard Rules, Yellow Unique Specification, and FireRed Retention Specification** define approved requirements, governance, canon, and integration policy.
2. **Current Project State** identifies the broad operational development boundary without duplicating fix-by-fix acceptance evidence.
3. **Acceptance Log and linked detailed acceptance records** establish what gameplay progress the user has explicitly accepted and what regression baseline must be preserved.
4. **The live GitHub repository and directly inspected current files** remain authoritative for what is actually implemented now.

If live code and the approved requirement documents appear to conflict:

- inspect the relevant code and recent decision history;
- determine whether the code is incomplete, experimental, regressed, or reflects an approved project-level change;
- do not silently redefine the project from implementation drift;
- update this master document only when a new major project-level decision has actually been approved.

Routine gameplay acceptance is closed out under Hard Rule `DOC-006`: acceptance evidence belongs in the Acceptance Log, while Current Project State changes only when the broad operational boundary changes. Routine acceptance does not by itself require a Master Document edit.

The latest approved project-level decision remains authoritative for project direction. The latest verified live repository state remains authoritative for implementation state.

---

# 63. Master Project Summary

Pokémon Golden Yellow is a FireRed-based Game Boy Advance project that faithfully rebuilds Pokémon Yellow’s complete Kanto campaign using modern mechanics and a persistent Partner Pikachu, continues through a Gen I-focused Sevii Islands bridge, advances the world approximately three years, transitions into a Crystal-led Johto campaign, later returns the player to an evolved Kanto for a full 16-badge journey, and culminates in Mt. Silver and an Emerald-inspired endgame.

The engine combines:

- Generation III architecture;
- key Generation II time/world systems;
- selected later-generation battle improvements;
- Golden Yellow-specific cross-region systems.

The game is designed as a single-player-complete #001–#251 experience with no version exclusives, no mandatory trade evolutions, one persistent save, and compatibility with both physical GBA hardware and major emulators.

