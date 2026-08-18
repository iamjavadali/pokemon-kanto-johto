# Pokémon Golden Yellow — FireRed Retention Specification

**Document type:** FireRed/Gen III modernization retention specification  
**Project:** Pokémon Golden Yellow  
**Campaign scope:** Yellow-era Kanto, Sevii Islands, and systems intended to remain available for later Johto integration  
**Narrative authority:** Pokémon Yellow for the initial Kanto story  
**Modernization reference:** Pokémon FireRed/LeafGreen, Generation III systems, and `pokeemerald-expansion`  
**Target implementation:** `pokeemerald-expansion`  
**Status:** Approved reference specification  
**Purpose:** Define the FireRed/Gen III systems, quality-of-life features, UI improvements, facilities, and expanded content that should survive the conversion of Kanto into a modernized Pokémon Yellow campaign.

---

# 1. Purpose

Pokémon Golden Yellow is not intended to remove FireRed's advancements merely because the narrative is being converted to Pokémon Yellow.

The governing rule is:

> **Pokémon Yellow determines the Kanto story. FireRed/Gen III provides compatible modernization. `pokeemerald-expansion` may supersede either when it provides a better implementation without violating Yellow canon.**

FireRed features should therefore be separated into three categories.

## 1.1 MUST RETAIN

The feature is an approved part of Golden Yellow's modern GBA experience.

It must not disappear during Yellow story conversion.

## 1.2 SHOULD RETAIN / MODERNIZE

The feature provides meaningful value and should remain unless:

- `pokeemerald-expansion` already provides a better replacement;
- hardware/save constraints create a substantial problem;
- a later Golden Yellow system supersedes it.

## 1.3 MAY RETAIN / REPURPOSE

The original FRLG feature is not essential, but its map, facility, UI, or concept may be useful.

---

# 2. Conflict Resolution

When FireRed and Yellow conflict:

1. Yellow-specific narrative canon wins.
2. Compatible FireRed modernization remains.
3. `pokeemerald-expansion` may replace outdated FRLG mechanics with superior implementations.
4. Single-player completion requirements override multiplayer or external-distribution dependencies.
5. Useful FRLG locations and systems should be adapted rather than deleted unnecessarily.

Example:

**Yellow:** Oak catches Pikachu and the Rival takes Eevee.  
**FireRed:** modern GBA Oak's Lab presentation.  
**Golden Yellow:** Yellow story sequence using modern GBA presentation.

---

# 3. Pillar 1 — Gen III Battle & Pokémon Systems

Yellow's story must run inside a modern battle engine.

Golden Yellow MUST NOT revert to Generation I battle mechanics merely for narrative fidelity.

---

# 4. Abilities — MUST RETAIN

Pokémon Abilities are part of the modern battle baseline.

Yellow trainer teams and wild Pokémon use the modern Ability system.

The exact Ability configuration should be governed by the project's Pokémon data and `pokeemerald-expansion`.

Yellow canon determines species identity, not the absence of Abilities.

---

# 5. Natures — MUST RETAIN

Natures remain enabled for:

- Partner Pikachu;
- gifts;
- wild Pokémon;
- trainer Pokémon where applicable;
- breeding;
- later Johto progression.

Narrative gifts must not disable the Nature system simply because their source game predates it.

---

# 6. IVs and EVs — MUST RETAIN

Use the modern Generation III-derived IV/EV architecture supported by the expansion.

Do not reproduce Generation I stat-experience or DV mechanics.

---

# 7. Held Items — MUST RETAIN

The modern held-item system remains active.

Examples include:

- Leftovers;
- Quick Claw;
- King's Rock;
- Light Ball;
- Choice Band;
- Metal Coat;
- Dragon Scale;
- type-boosting items;
- status and recovery items;
- Berries.

Yellow boss teams may eventually receive appropriate held items as part of modernization.

Species composition remains Yellow canon.

---

# 8. Pokémon Gender — MUST RETAIN

Gender mechanics remain active.

This is required for:

- breeding;
- gender-specific mechanics;
- later Johto systems;
- modern Pokémon data.

---

# 9. Friendship — MUST RETAIN

Modern friendship architecture remains active.

Partner Pikachu additionally has Yellow-specific narrative dependencies on its friendship/happiness state.

Ordinary Pokémon friendship remains a normal global mechanic.

---

# 10. Expanded Movesets — MUST RETAIN

Pokémon are not limited to Generation I learnsets.

Golden Yellow should use modernized movesets designed around:

- the Physical/Special split;
- Ability synergy;
- useful STAB;
- progression balance;
- TM/Tutor availability;
- species identity.

Yellow's trainer roster does not imply Yellow-era movesets.

---

# 11. Physical/Special Split — MUST USE MODERN VERSION

Original FireRed still used type-based Physical/Special classification.

Golden Yellow MUST use the modern **per-move Physical/Special split**.

This is an intentional improvement beyond original FRLG.

---

# 12. Double Battles — MUST RETAIN

Double Battles remain a supported core battle format.

They may be used for:

- Jessie and James;
- Twins;
- couples;
- paired Rocket trainers;
- selected rematches;
- Johto encounters;
- endgame facilities.

## 12.1 Yellow Integration Rule

A Yellow single battle should not automatically become a Double Battle.

Use Double Battles when they strengthen the scene.

Jessie and James are the strongest Yellow-specific candidates.

---

# 13. Weather — MUST RETAIN

Battle weather remains supported.

This includes modern interactions for:

- rain;
- sun;
- sandstorm;
- hail or expansion-equivalent weather where appropriate;
- Abilities;
- weather-based moves.

Overworld weather should be used selectively.

Kanto should not be transformed into Hoenn merely because the engine supports weather.

---

# 14. Modern Evolution Architecture — MUST RETAIN

Use the expansion's evolution system rather than Generation I evolution limitations.

This allows:

- friendship evolution;
- held-item evolution;
- time-based evolution;
- location-based evolution;
- alternative trade evolutions.

Golden Yellow's single-player evolution rules may extend this system later.

---

# 15. Modern Poké Balls — MUST RETAIN

Later-generation Poké Ball types and behavior may remain available where balanced appropriately.

The Yellow story does not require a return to only Generation I Ball types.

---

# 16. Pillar 2 — Movement & Overworld Quality of Life

---

# 17. Running Shoes — MUST RETAIN

Golden Yellow includes Running Shoes.

They must not be removed because original Pokémon Yellow lacked running.

## 17.1 FRLG Baseline

FireRed gives the Running Shoes after Brock through one of Professor Oak's aides.

## 17.2 Golden Yellow Rule

The system is mandatory.

The exact acquisition moment MAY be adjusted later if a more natural Yellow-integrated event is designed.

Until deliberately changed, the FRLG acquisition point is a valid baseline.

---

# 18. Bicycle — MUST RETAIN

Keep the modern GBA Bicycle system.

Yellow story progression around acquiring the Bicycle may be preserved or adapted without removing the feature.

---

# 19. Fly Convenience — MUST RETAIN / EXPAND

Use modern Fly infrastructure.

Golden Yellow should retain FRLG-style improvements such as useful route Pokémon Center destinations where supported.

The final system must eventually scale to:

- Kanto;
- Sevii;
- Johto.

The implementation should therefore avoid assumptions that only one region exists.

---

# 20. Modern Map Transitions — MUST RETAIN

Use GBA-quality:

- doors;
- stairs;
- warps;
- caves;
- fades;
- object movement;
- map transitions.

Yellow scenes should be translated into modern GBA choreography.

Do not emulate Game Boy transition limitations.

---

# 21. Environmental Field Interactions — SHOULD RETAIN

Useful modern interactions such as:

- Rock Smash rocks;
- Cut objects;
- Strength puzzles;
- Surf;
- Waterfall where later appropriate;
- other expansion-supported field interactions

should remain where they improve exploration.

They must not create arbitrary progression changes that contradict Yellow canon.

---

# 22. Pillar 3 — Bag, Items, Berries & Move Management

---

# 23. Modern Bag Architecture — MUST RETAIN

Golden Yellow must use a modern organized inventory.

Do not regress to Generation I's limited item-storage design.

The player should have logically separated categories for:

- ordinary items;
- Key Items;
- Poké Balls;
- TMs/HMs or equivalent move-teaching items;
- Berries.

## 23.1 Implementation Rule

The exact FRLG visual structure does not need to be cloned if `pokeemerald-expansion` provides a superior equivalent.

Preserve functionality and usability.

---

# 24. TM Management UI — MUST RETAIN

FRLG's TM Case represents a major improvement over Yellow's inventory handling.

Golden Yellow requires equivalent modern TM organization.

The interface should provide useful information such as:

- TM number;
- move name;
- move type;
- move details where supported;
- compatibility indication.

## 24.1 TM Consumption Rule

**TMs are NOT reusable under the current Golden Yellow design.**

Using a TM consumes it unless a later explicitly approved project decision changes this rule.

The modern TM interface remains even though TM consumption remains limited.

---

# 25. HM / Field Move Management — RETAIN, SUBJECT TO LATER DESIGN

Field-move infrastructure remains required.

The exact long-term HM model may later be modernized.

Until that decision is made:

- do not remove required field progression;
- do not unnecessarily duplicate HM systems;
- preserve compatibility with the current expansion implementation.

---

# 26. Berry System — MUST RETAIN

Berries are part of Golden Yellow.

Retain:

- healing Berries;
- status-curing Berries;
- PP-restoring Berries;
- battle-trigger Berries;
- stat-related Berries where supported;
- held-Berry effects.

---

# 27. Berry Storage UI — MUST RETAIN FUNCTIONALLY

FRLG uses the Berry Pouch.

Golden Yellow must preserve equivalent dedicated Berry organization.

If `pokeemerald-expansion` provides a Berry pocket directly inside the Bag, that implementation may be used instead.

The requirement is:

> Berries must remain organized and convenient to access.

It is not necessary to reproduce the literal FRLG Berry Pouch Key Item if the target engine already provides a better design.

---

# 28. Berry Availability — KEEP SYSTEM, IMPROVE ECONOMY

Do not reproduce FRLG's limited Berry availability as a design restriction.

Golden Yellow spans a much longer campaign.

Renewable Berry sources SHOULD eventually exist through suitable systems such as:

- Sevii resources;
- Johto farms;
- Berry-growing areas;
- timed regeneration;
- NPC gifts.

Exact implementation belongs in a later economy specification.

---

# 29. Move Tutors — MUST RETAIN

Move Tutors remain part of Golden Yellow.

They are valuable for supporting modernized movepools across Pokémon #001–251.

Specific Tutor:

- locations;
- costs;
- repeatability;
- move lists

may be revised later.

---

# 30. Move Reminder — MUST RETAIN

The player must have access to a modern Move Reminder/relearner.

FRLG's Two Island Move Reminder is a valid location and may be retained.

The exact payment method MAY be changed later.

---

# 31. Move Deleter — SHOULD RETAIN

Keep Move Deleter functionality while traditional HM restrictions remain relevant.

If a later field-move redesign removes the need for HM deletion restrictions, this NPC may be adapted or repurposed.

---

# 32. Cape Brink Special Tutor — SHOULD RETAIN / ADAPT

FRLG's Cape Brink tutor provides special starter-line moves.

Golden Yellow should retain Cape Brink as a special Move Tutor location.

However, Golden Yellow's actual starter is Partner Pikachu.

Bulbasaur, Charmander, and Squirtle are Yellow story gifts.

Therefore FRLG's original "chosen starter" logic MUST NOT be copied blindly.

Eligibility should later be redesigned around Golden Yellow's gift starter lines or another appropriate requirement.

---

# 33. Pillar 4 — Help, Tutorials & Player Guidance

---

# 34. Viridian Old Man — LOCKED GOLDEN YELLOW INTEGRATION

This interaction has an explicit Golden Yellow design.

## 34.1 Yellow Context

The Viridian Old Man remains part of early Viridian progression and world flavor.

## 34.2 FireRed Upgrade

Teachy TV is retained.

## 34.3 Golden Yellow Tutorial Rule

**The Old Man MUST NOT perform another Poké Ball catching demonstration.**

The player has already observed Professor Oak encounter and catch Pikachu during the Yellow opening.

Repeating another mandatory catching demonstration in Viridian would be redundant.

## 34.4 Required Golden Yellow Interaction

The Old Man should instead:

1. recognize or address the player's early journey;
2. provide a short piece of useful advice;
3. introduce the Teachy TV as a tool containing beginner guidance;
4. give the player the Teachy TV;
5. allow progression to continue immediately.

The scene should be concise.

## 34.5 Teachy TV — MUST RETAIN

Teachy TV remains available for players who want additional tutorials.

The tutorial therefore becomes **optional and player-controlled** rather than replaying information the opening has already demonstrated.

This is a canonical example of Golden Yellow integration:

**Yellow opening teaches catching context**  
+ **FRLG Teachy TV**  
− **redundant Old Man catching demonstration**

---

# 35. Help System — SHOULD RETAIN

FRLG's contextual Help System is useful, especially for new players.

Golden Yellow SHOULD retain an equivalent help/reference feature if compatible with the expansion's controls.

The exact FRLG L/R button mapping is not locked.

If those buttons are needed for other modern systems, the help interface may use another access method.

---

# 36. Startup Control Guidance — MAY RETAIN

Basic startup/control guidance may remain if it fits cleanly.

It is lower priority than:

- Yellow intro;
- Teachy TV;
- proper menus.

Do not allow a long tutorial sequence to delay the Yellow opening unnecessarily.

---

# 37. Pillar 5 — Utility & Progression Systems

---

# 38. Vs. Seeker — MUST RETAIN

The Vs. Seeker is a core Golden Yellow rematch system.

The original FRLG concept should be preserved:

- compatible trainers can be challenged again;
- trainers may gain stronger rematch teams;
- rematches provide renewable EXP and money.

## 38.1 Golden Yellow Expansion

Future rematch tiers should support the longer timeline.

Suggested architecture:

**Tier 1** — original Yellow battle  
**Tier 2** — later Kanto  
**Tier 3** — post-League / Sevii  
**Tier 4** — three-years-later Kanto  
**Tier 5** — selected endgame trainers

Exact trainer teams belong in a later trainer specification.

---

# 39. Vs. Seeker Acquisition — SHOULD RETAIN FRLG BASELINE

FRLG gives the Vs. Seeker in Vermilion City's Pokémon Center.

This remains a sensible acquisition point.

It MAY be moved later if story integration provides a clearly superior location.

---

# 40. Fame Checker — MUST RETAIN AND EXPAND

The Fame Checker is a protected Golden Yellow feature.

It must not disappear during Yellow story conversion.

Potential tracked characters include:

- Professor Oak;
- Rival;
- Brock;
- Misty;
- Lt. Surge;
- Erika;
- Koga;
- Sabrina;
- Blaine;
- Giovanni;
- Jessie;
- James;
- Lorelei;
- later Johto characters.

## 40.1 Long-Term Role

The Fame Checker should be designed to scale beyond its original FRLG purpose.

It may become a cross-region character/history archive showing:

- notable facts;
- NPC testimony;
- character relationships;
- changing reputations;
- continuity across the three-year transition.

The exact number of entries per character does not need to remain identical to FRLG if a better structure is required.

The feature itself is mandatory.

---

# 41. Itemfinder — MUST RETAIN FUNCTIONALITY

The hidden-item discovery system remains available.

If `pokeemerald-expansion` provides a better modern implementation than original FRLG, use the expansion version.

The feature matters more than exact FRLG behavior.

---

# 42. Professor Oak's Aide Reward System — SHOULD RETAIN

FRLG rewards Pokédex progression through Professor Oak's aides.

This is a useful exploration incentive.

## 42.1 FRLG Baseline Rewards

The original structure includes milestones such as:

- Running Shoes after Brock;
- Flash for Pokédex progress;
- Everstone;
- Itemfinder;
- Amulet Coin;
- Exp. Share.

## 42.2 Golden Yellow Rule

Retain the **milestone reward architecture**.

Exact:

- species thresholds;
- locations;
- reward items

MAY later be adjusted for Golden Yellow's #001–251 structure.

Do not remove the aide system merely because Yellow originally lacked it.

---

# 43. Exp. Share — MUST RETAIN

The Exp. Share remains part of Golden Yellow.

Its exact mechanics should follow the approved `pokeemerald-expansion` configuration rather than blindly reproducing FRLG behavior.

---

# 44. Pillar 6 — Pokédex, Menus, Storage & Progress Tracking

---

# 45. Modern Pokédex — MUST RETAIN AND EXPAND

Golden Yellow must retain modern GBA Pokédex presentation.

Required concepts include:

- Seen;
- Caught;
- species data;
- searching;
- scrolling/navigation;
- location information where practical.

## 45.1 Golden Yellow Expansion

The final system should eventually support:

- Kanto view;
- Johto view;
- combined #001–251 view;
- encounter guidance;
- evolution guidance;
- time-of-day hints;
- special-condition hints.

FRLG provides the foundation.

Golden Yellow expands it.

---

# 46. Start Menu — MUST RETAIN

Use a modern GBA Start Menu.

Core entries should include appropriate equivalents of:

- Pokédex;
- Pokémon;
- Bag;
- player/Trainer Card;
- Save;
- Options;
- contextual systems.

Yellow narrative fidelity MUST NOT cause the UI to regress toward Generation I.

---

# 47. Trainer Card — MUST RETAIN AND EXPAND

The Trainer Card should remain part of the player-information interface.

It should support:

- Trainer name;
- ID;
- play time;
- money where appropriate;
- badges;
- Hall of Fame status;
- other modern progression records.

## 47.1 Future Cross-Region Requirement

Architecture should eventually accommodate:

- eight initial Kanto badges;
- eight Johto badges;
- complete 16-badge progression.

---

# 48. Trainer Card Achievement Stickers — MAY RETAIN

FRLG includes optional accomplishment stickers through Four Island.

These may be retained if implementation cost is low.

They could eventually reward achievements such as:

- Hall of Fame completions;
- Eggs hatched;
- Pokédex progress;
- Frontier accomplishments.

Not core to the Yellow story.

---

# 49. Pokémon Storage System — MUST RETAIN

Use robust modern PC box storage.

Do not reproduce Yellow's limited PC behavior.

This is mandatory for:

- #001–251 completion;
- multi-region progression;
- breeding;
- long-term party management.

---

# 50. Item PC — MAY RETAIN

If already supported cleanly, Item PC functionality may remain.

It is not a defining Golden Yellow requirement if modern Bag capacity makes it largely unnecessary.

---

# 51. Quest Log / Resume System — MUST RETAIN

The FRLG Quest Log/resume concept is a protected Golden Yellow feature.

It must not disappear during Yellow story conversion.

Golden Yellow should retain or recreate an equivalent recent-progress summary system.

Potential entries include:

- defeated a Gym Leader;
- received a Yellow gift starter;
- defeated Jessie and James;
- cleared Silph Co.;
- became Champion;
- completed a Sevii objective;
- began the Johto journey;
- defeated a Johto Gym;
- returned to later Kanto.

## 51.1 Long-Term Requirement

The Quest Log must be designed to scale across:

- Yellow-era Kanto;
- Sevii;
- three-year transition;
- Johto;
- later Kanto;
- final endgame.

## 51.2 Implementation Rule

Do not lock Golden Yellow to FRLG's exact storage limits or presentation.

`pokeemerald-expansion` or custom Golden Yellow architecture may implement the feature differently.

The mandatory requirement is:

> **The player must retain access to a meaningful recent-progress/resume system appropriate to the full multi-region campaign.**

Save-memory and hardware limits must be considered during implementation.

---

# 52. Pillar 7 — Kanto Modernization Additions

These features enhance the Yellow Kanto campaign without changing its narrative identity.

---

# 53. Route and Pokémon Center Fly Destinations — SHOULD RETAIN

Useful FRLG travel conveniences should remain where technically supported.

Golden Yellow's map/travel architecture should eventually become more flexible, not less.

---

# 54. Oak Aide Map Presence — SHOULD RETAIN

FRLG places Oak aides at strategic Kanto gates and facilities.

These NPCs provide:

- Pokédex milestone rewards;
- worldbuilding;
- incentives to catch Pokémon.

They may remain integrated into the Yellow campaign.

---

# 55. Route 2 / Flash Reward — SHOULD RETAIN

The Oak aide milestone associated with Flash remains a useful modern progression reward.

Exact requirements may later be adjusted.

---

# 56. Route 10 / Everstone Reward — SHOULD RETAIN

Retain the aide/milestone concept.

---

# 57. Route 11 / Itemfinder Reward — SHOULD RETAIN

Retain.

---

# 58. Route 16 / Amulet Coin Reward — SHOULD RETAIN

Retain.

---

# 59. Route 15 / Exp. Share Reward — SHOULD RETAIN

Retain the milestone reward concept.

Exact threshold may be rebalanced around Golden Yellow's Pokédex.

---

# 60. Cerulean Cave Modernization — SHOULD RETAIN

FRLG-style environmental additions such as Rock Smash interactions may remain where they do not interfere with:

- Mewtwo access;
- Yellow canonical progression;
- dungeon readability.

---

# 61. Modern Pokémon Center / Mart UI — MUST RETAIN

Golden Yellow should use modern GBA:

- healing interface;
- shop interface;
- PC interface;
- menus;
- item purchasing systems.

Yellow narrative adaptation does not require Gen I UI recreation.

---

# 62. Pillar 8 — Sevii Islands & Postgame Content

The Sevii Islands are a required Golden Yellow campaign phase.

They serve as the bridge between Yellow-era Kanto and the later Crystal-era transition.

---

# 63. Sevii Islands — MUST RETAIN

Retain:

- One Island;
- Two Island;
- Three Island;
- Four Island;
- Five Island;
- Six Island;
- Seven Island;
- ferry infrastructure;
- island towns;
- island dungeons;
- postgame facilities.

---

# 64. Celio — MUST RETAIN

Celio and the Pokémon Network Center should remain important Sevii characters/infrastructure.

However, Golden Yellow should reinterpret external-trading motivations where necessary.

---

# 65. Tri-Pass — MUST RETAIN FUNCTIONALLY

Retain the early Sevii travel progression represented by the Tri-Pass.

Exact item presentation may be modernized if the final regional-travel system evolves.

---

# 66. Rainbow Pass — MUST RETAIN FUNCTIONALLY

Retain the later expanded Sevii access represented by the Rainbow Pass.

---

# 67. Network Machine Quest — RETAIN / REFRAME

FRLG's Ruby/Sapphire Network Machine quest provides useful:

- exploration;
- progression;
- Celio development;
- island unlock structure.

Golden Yellow SHOULD retain this quest architecture.

However, the final narrative reward should not depend on enabling external cartridge trading.

The quest may instead support:

- regional communications;
- Golden Yellow travel systems;
- storage connectivity;
- Johto foreshadowing;
- another internal single-player system.

Exact reframing belongs to the Sevii implementation phase.

---

# 68. Team Rocket Sevii Story — MUST RETAIN

FireRed's postgame Rocket storyline is strategically important to Golden Yellow.

It should provide continuity:

**Giovanni defeated in Kanto**  
→ **Rocket remnants persist in Sevii**  
→ **organization survives the intervening years**  
→ **Crystal-era Team Rocket later re-emerges in Johto**

This storyline should be adapted deliberately to support the combined timeline.

---

# 69. Rocket Warehouse — MUST RETAIN

The Rocket Warehouse and associated Sevii Rocket progression remain part of the campaign.

Exact trainer teams and dialogue may later be adapted to better foreshadow Johto.

---

# 70. Four Island Breeding Day Care — MUST RETAIN

Four Island's two-Pokémon Day Care and breeding functionality remain.

This provides a useful bridge toward Crystal-era breeding systems.

Breeding should not become mandatory for completing Yellow Kanto.

---

# 71. Lorelei Four Island Content — SHOULD RETAIN

Lorelei's connection to Four Island provides valuable worldbuilding.

Retain:

- Lorelei's hometown connection;
- Icefall Cave;
- her Sevii story role.

This helps League characters exist outside the League rooms.

---

# 72. Trainer Tower — SHOULD RETAIN / REPURPOSE

Trainer Tower is valuable battle-facility infrastructure.

Because Golden Yellow also plans an Emerald-inspired Battle Frontier, Trainer Tower should not become redundant.

Possible roles include:

- pre-Frontier challenge facility;
- timed challenge facility;
- special rematch tower;
- Sevii battle facility;
- progression requirement toward later Frontier access.

Final role should be decided during endgame design.

Do not delete the location prematurely.

---

# 73. Altering Cave — MAY RETAIN / REPURPOSE

Original FRLG external-event behavior should not be preserved as a dependency.

The cave itself may become useful Golden Yellow content.

Possible future uses:

- rotating encounters;
- time-based rare Pokémon;
- special evolution area;
- postgame Pokémon access;
- challenge area.

No final role is locked here.

---

# 74. Elite Four & Champion Rematches — MUST RETAIN AND EXPAND

Stronger League rematches are required.

Potential Golden Yellow tiers:

1. first Yellow League;
2. post-Sevii rematch;
3. later post-Johto / evolved-Kanto rematch;
4. optional final endgame rematch.

Modern rematches may use:

- stronger AI;
- held items;
- evolved Pokémon;
- improved moves;
- higher levels.

Yellow team identity should still influence the initial Kanto League structure where applicable.

---

# 75. Pillar 9 — Yellow + FireRed Integration Rules

This section governs all conflicts between Yellow narrative fidelity and FireRed modernization.

---

# 76. Yellow Story Wins

When FireRed story content contradicts a Yellow-specific event, Yellow wins.

Examples:

- Oak catches Pikachu.
- Rival gets Eevee.
- Bulbasaur/Charmander/Squirtle are gifts.
- Jessie and James replace relevant generic Rocket encounters.
- Giovanni retains Persian.
- Moltres remains in Victory Road.

---

# 77. FireRed Upgrade Survives When Compatible

FireRed features should remain when they do not contradict the Yellow story.

Examples:

- Running Shoes;
- Bag organization;
- Berry management;
- TM management;
- Teachy TV;
- Vs. Seeker;
- Fame Checker;
- Quest Log;
- modern Pokédex;
- modern storage;
- Sevii Islands.

---

# 78. Expansion May Supersede FRLG Implementation

Because Golden Yellow uses `pokeemerald-expansion`, the project should not recreate an inferior FRLG subsystem solely for authenticity.

Examples:

- modern Physical/Special split replaces FRLG's type-based system;
- expansion battle AI may replace FRLG AI;
- expansion Bag pockets may replace literal FRLG TM Case/Berry Pouch containers;
- modern evolution methods may supersede FRLG restrictions.

Preserve the **player-facing capability and design value**.

Do not unnecessarily duplicate systems.

---

# 79. Single-Player Completeness Overrides Connectivity

No required content may depend on:

- another cartridge;
- another player;
- Wireless Adapter;
- link cable;
- Mystery Gift;
- historical event distribution.

External connectivity MAY remain as optional functionality if technically safe and inexpensive.

---

# 80. Mystery Gift Content — INTERNALIZE IF RETAINED

Content such as event-island access may be retained.

But any worthwhile content must gain an internal single-player unlock path.

Historical distribution items must not remain the only access mechanism.

---

# 81. Wireless Minigames — NOT CORE

Original FRLG multiplayer features such as:

- Pokémon Jump;
- Dodrio Berry Picking;
- Berry Crush

are not mandatory Golden Yellow features.

They MAY:

- remain if functional;
- be converted into single-player activities;
- be repurposed;
- be removed if they create unnecessary maintenance.

No progression requirement may depend on them.

---

# 82. Old Man Integration — LOCKED SPECIAL CASE

Viridian Old Man behavior is explicitly defined:

**DO NOT replay a catching tutorial.**

Golden Yellow already teaches catching context through Oak's opening Pikachu encounter.

The Old Man instead:

- provides brief advice;
- gives Teachy TV;
- allows the player to continue.

Teachy TV contains optional additional tutorials.

This rule should remain stable unless deliberately revised.

---

# 83. Jessie & James Integration — LOCKED PRINCIPLE

Yellow defines:

- encounter placement;
- characters;
- battle identity;
- team progression.

Gen III provides:

- modern battle engine;
- Double Battle support;
- modern moves;
- Abilities;
- presentation.

Suitable Jessie/James fights MAY become Double Battles while preserving their Yellow narrative placement.

---

# 84. Yellow Boss + Modern Battle Integration

Yellow determines recognizable team species.

Modern systems may improve:

- movesets;
- AI;
- held items;
- Abilities;
- Natures;
- IV/EV configuration;
- battle presentation.

Do not replace a Yellow-defining team solely because FireRed used a different roster.

---

# 85. Yellow Gift + Modern Storage Integration

Yellow determines:

- NPC;
- location;
- requirement;
- species;
- gift level.

Modern engine determines:

- party-full handling;
- PC storage;
- nickname flow;
- Pokédex registration;
- Pokémon metadata.

---

# 86. Partner Pikachu + Modern Engine Integration

Yellow determines:

- Partner Pikachu's narrative identity;
- follower relationship;
- happiness relevance;
- evolution refusal;
- required reaction scenes.

Golden Yellow's engine determines:

- persistent Partner identification;
- follower rendering;
- map transitions;
- cutscene suspension;
- modern friendship storage.

---

# 87. FireRed Retention Map/System Index

## Pallet / Opening

Retain:
- modern map presentation;
- modern battle transition;
- menus.

Override:
- FireRed starter story.

Use Yellow opening instead.

## Viridian City

Retain:
- Teachy TV.

Modify:
- Old Man interaction.

Remove:
- redundant catching demonstration.

## Pewter / Route 3

Retain:
- Running Shoes system;
- FRLG-quality map flow.

## Kanto Gates / Routes

Retain:
- Oak aide milestones;
- modern Fly destinations;
- Itemfinder progression;
- Exp. Share reward structure.

## Vermilion

Retain:
- Vs. Seeker acquisition;
- modern Pokémon Center systems.

Overlay:
- Yellow Officer Jenny/Squirtle event.

## Celadon

Retain:
- modern Game Corner/item systems where appropriate;
- improved Itemfinder behavior.

Overlay:
- Yellow Jessie/James and Giovanni teams where applicable.

## Fuchsia

Retain:
- modern Safari Zone/GBA interface where applicable.

Overlay:
- Yellow Koga team.

## Saffron

Retain:
- FRLG-quality Silph navigation/presentation.

Overlay:
- Yellow Rival;
- Jessie/James;
- Giovanni Persian progression.

## Cinnabar

Retain:
- modern map/Gym presentation.

Overlay:
- Yellow Blaine team.

## Viridian Gym

Retain:
- FRLG-quality Gym/map presentation.

Overlay:
- Yellow Giovanni team and Persian identity.

## Victory Road

Retain:
- GBA dungeon presentation.

Override:
- FireRed Moltres relocation.

Use Yellow-canonical Victory Road Moltres.

## Pokémon League

Retain:
- GBA battle presentation;
- rematch infrastructure.

Use:
- Yellow-defined first-campaign team identity where required.

## Sevii

Retain and adapt:
- full island structure;
- Celio;
- ferry passes;
- Rocket story;
- breeding;
- tutors;
- Lorelei;
- facilities;
- rematches.

---

# 88. Explicit FireRed Features That MUST NOT Be Lost

The following are protected retention targets:

- Abilities
- Natures
- modern IV/EV system
- held items
- Pokémon genders
- friendship
- expanded movesets
- modern Physical/Special split through `pokeemerald-expansion`
- Double Battles
- weather
- modern evolution system
- Running Shoes
- Bicycle
- improved Fly
- modern Bag organization
- TM management interface
- Berry management interface
- Berry system
- Move Tutors
- Move Reminder
- Teachy TV
- Vs. Seeker
- Fame Checker
- Quest Log / Resume System
- Itemfinder
- Exp. Share
- modern Pokédex
- modern Start Menu
- Trainer Card
- modern PC/storage
- modern Pokémon Center/Mart UI
- Sevii Islands
- Celio
- Tri-Pass/Rainbow Pass progression
- Team Rocket Sevii storyline
- Rocket Warehouse
- Four Island breeding
- Elite Four/Champion rematches

---

# 89. Strong SHOULD-RETAIN Features

These should remain unless later architecture provides a stronger replacement:

- Professor Oak's aide milestone system;
- contextual Help System;
- Lorelei's Four Island content;
- Trainer Tower;
- Cape Brink special tutoring;
- expanded route Fly points;
- additional environmental interactions.

---

# 90. Optional / Repurpose Features

These are not core requirements:

- Trainer Card achievement stickers;
- Berry Crush;
- Pokémon Jump;
- Dodrio Berry Picking;
- Wireless Adapter-specific UI;
- Union Room dependence;
- original external Mystery Gift behavior;
- Altering Cave's unreleased external encounter switching;
- external event-ticket distribution.

Their assets, locations, or concepts MAY be reused.

---

# 91. Preservation Test

For every Yellow conversion pass, ask two questions.

## Question 1

> **Does the existing FireRed feature contradict a Yellow-specific story event?**

If yes:

- preserve Yellow story;
- retain as much compatible FireRed functionality as possible.

## Question 2

> **Does removing this FireRed feature make Golden Yellow less modern without improving Yellow fidelity?**

If yes:

**do not remove it.**

---

# 92. Final Golden Yellow Kanto Experience

A successful Golden Yellow implementation should feel like:

**Pokémon Yellow's story**  
running through  
**a modern GBA Pokémon engine**  
with  
**FireRed-quality exploration and usability**  
and  
**later-generation mechanical improvements**.

The player should be able to experience:

**Oak catching Pikachu**  
→ while using modern battle presentation

**Partner Pikachu following the player**  
→ while using Running Shoes and modern map systems

**Yellow starter gifts**  
→ while using modern PC/storage handling

**Jessie and James**  
→ while benefiting from Double Battle support

**Yellow Gym teams**  
→ using Abilities, modern moves, held items, and the Physical/Special split

**Yellow Kanto completion**  
→ followed naturally by FireRed's Sevii expansion

without feeling that modern features were removed merely for historical accuracy.

---

# 93. Governing Summary

Pokémon Yellow determines:

- narrative;
- starter story;
- Partner Pikachu;
- Rival progression;
- anime events;
- Yellow-specific boss identity;
- story-specific legendary placement.

FireRed/Generation III contributes:

- GBA presentation;
- movement improvements;
- inventory improvements;
- modern Pokémon systems;
- player guidance;
- rematches;
- utilities;
- Fame Checker;
- Quest Log;
- expanded Pokédex/UI;
- postgame content;
- Sevii Islands.

`pokeemerald-expansion` may improve or replace the underlying FRLG implementation where technically superior.

The final objective is:

> **Preserve everything that makes Pokémon Yellow unique while retaining everything useful that makes FireRed and modern Generation III gameplay better.**

---

# 94. Source References

## Primary FireRed technical/reference source

- `pret/pokefirered`
  - map scripts and object/event architecture
  - Bag/Key Item systems
  - Teachy TV
  - Vs. Seeker
  - Fame Checker
  - Quest Log
  - Oak aide reward infrastructure
  - Sevii Islands
  - Trainer Tower
  - rematch and postgame systems

Repository: `https://github.com/pret/pokefirered`

## Modern engine reference

- `pokeemerald-expansion`
  - modern battle systems
  - Physical/Special split
  - expanded mechanics
  - modern evolution/data infrastructure
  - current target implementation architecture

## Golden Yellow project authorities

- `Pokemon_Golden_Yellow_Project_Master.md`
- `Pokemon_Golden_Yellow_Hard_Rules.md`
- `Pokemon_Golden_Yellow_Yellow_Unique_Specification.md`

The latest explicitly approved project decisions override older conflicting statements in project documents. In particular, the current Golden Yellow design keeps modern TM management while **not enabling reusable TMs**.
