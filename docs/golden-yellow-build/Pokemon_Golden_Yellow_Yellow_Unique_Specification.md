# Pokémon Golden Yellow — Yellow Unique Specification

**Document type:** Yellow-exclusive narrative and progression specification  
**Project:** Pokémon Golden Yellow  
**Campaign scope:** Initial Kanto campaign, Pallet Town → first Champion victory  
**Source authority:** Pokémon Yellow  
**Target implementation:** Modern GBA / `pokeemerald-expansion` architecture  
**Status:** Approved reference specification  
**Purpose:** Define the Pokémon Yellow story identity that must survive the FireRed/Gen III modernization layer.

---

# 1. Purpose

This specification defines the narrative, progression, character, Partner Pikachu, Rival, anime-inspired, and boss elements that distinguish **Pokémon Yellow** from a standard Red/Blue/FireRed Kanto campaign.

It intentionally does **not** attempt to recreate Pokémon Yellow as a literal Game Boy port.

Pokémon Golden Yellow is a **modernized Yellow campaign**.

The governing rule is:

> **Preserve Yellow-exclusive narrative identity and progression while retaining compatible FireRed/Gen III/expansion upgrades.**

A newer feature should not be removed merely because Pokémon Yellow did not contain it.

Examples:

- Teachy TV may remain.
- Running Shoes may remain.
- modern battle mechanics remain.
- modern menus remain.
- modern storage and gift handling remain.
- modern Pokémon follower architecture may be used.
- modern maps and GBA presentation remain.

However, these systems must not silently replace a Yellow-specific story event.

---

# 2. Integration Classification

Each Yellow-specific area should be understood using three categories.

## 2.1 Yellow Canon — MUST Preserve

Story or progression behavior that materially establishes Pokémon Yellow's identity.

Examples:

- Oak catching Pikachu;
- Rival taking Eevee;
- Partner Pikachu becoming the player's starter;
- Yellow Rival Eevee branching;
- Jessie and James encounters;
- Yellow starter gifts;
- Yellow-specific boss compositions.

## 2.2 Modern Upgrade — RETAIN Where Compatible

A FireRed, Emerald, `pokeemerald-expansion`, or Golden Yellow improvement that does not contradict Yellow canon.

Examples:

- Running Shoes;
- Teachy TV;
- Abilities;
- Natures;
- Physical/Special split;
- improved AI;
- modern item/storage handling;
- GBA menus;
- better cutscene presentation;
- modern follower implementation.

## 2.3 Integration Rule

Defines how the Yellow event and modern system coexist.

The goal is not:

> Replace FireRed with Yellow everywhere.

The goal is:

> Make the game unmistakably Pokémon Yellow while benefiting from later-generation improvements.

---

# 3. Pillar 1 — Yellow Opening & Starter Sequence

This sequence is the most important Yellow-specific story override in the game.

A standard FireRed three-starter opening MUST NOT replace it.

## 3.1 Pallet Town Departure

### Yellow Canon

When the player first attempts to leave Pallet Town toward Route 1:

1. The player advances into the northern grass area.
2. Professor Oak interrupts the player.
3. Oak approaches.
4. Oak notices a nearby wild Pikachu.
5. A special Pikachu encounter begins.
6. The Pikachu is Lv. 5.
7. Oak successfully secures/catches the Pikachu.
8. Oak tells the player that it is unsafe to travel without a Pokémon.
9. Oak directs the player back toward his laboratory.

Pokémon Yellow's Pallet script transitions into a dedicated Pikachu battle using the Yellow starter-Pikachu battle state and enemy level 5.

### Modern Upgrade

Golden Yellow MAY use:

- FRLG map geometry;
- modern GBA movement;
- modern battle transitions;
- improved animations;
- modern sound effects;
- more polished Oak choreography.

### Integration Rule

The visible event order must remain Yellow.

FireRed's normal:

**leave Pallet → Oak stops player → choose Bulbasaur/Charmander/Squirtle**

must not survive as the canonical opening.

---

# 4. Oak's Lab Starter Event

## 4.1 Eevee Is the Intended Lab Pokémon

### Yellow Canon

The Lab contains the Pokémon that will become the Rival's Eevee.

The player attempts to receive it.

The Rival interrupts and takes it first.

### Required Sequence

1. Rival expresses impatience about receiving a Pokémon.
2. Player moves toward the available Poké Ball.
3. Rival interrupts.
4. Rival physically approaches/intercepts.
5. Rival takes Eevee.
6. Oak permits Rival to keep Eevee.
7. Oak calls the player over.
8. Oak offers the Pikachu caught during the Pallet event.
9. Player receives that Pikachu.

### Prohibited Replacement

Do not present:

- Bulbasaur;
- Charmander;
- Squirtle

as selectable starters.

Those species belong to later Yellow gift events.

---

# 5. Canonical Partner Pikachu Creation

The Pikachu caught by Oak and subsequently given to the player becomes the game's **canonical Partner Pikachu**.

This identity must be distinct from ordinary Pikachu.

## 5.1 Narrative Requirements

Partner Pikachu MUST:

- originate from the opening Oak capture event;
- be received from Oak;
- be the player's sole Yellow-era starter;
- remain separately identifiable from ordinary Pikachu;
- become the follower after the first Rival battle;
- possess persistent happiness/friendship state;
- refuse normal evolution;
- participate in Yellow-specific reaction events.

## 5.2 Modern Integration

Golden Yellow does not need to recreate Yellow's internal Game Boy implementation.

The modern engine should use the project's canonical Partner Pikachu identity/helper mechanism.

The story invariant is what matters.

---

# 6. Rival Battle #1 — Oak's Lab

Rival's team:

- Eevee — Lv. 5

The result of this battle initializes the Rival Eevee evolution state.

## 6.1 Battle Result

### Player loses

Set:

`VAR_RIVAL_EEVEE_STATE = VAPOREON`

### Player wins

Set:

`VAR_RIVAL_EEVEE_STATE = FLAREON`

The optional early Route 22 battle can later promote only the Flareon state to Jolteon.

---

# 7. Pikachu Emergence After Rival Battle

This is a mandatory Yellow story scene.

It is not merely follower-engine initialization.

## 7.1 Yellow Canon

After Rival Battle #1:

1. Rival gives his departure dialogue.
2. Rival walks out.
3. Player watches him leave.
4. Rival disappears from the Lab.
5. Pikachu emerges from its Poké Ball.
6. Oak explains that Pikachu dislikes remaining inside its Poké Ball.
7. Pikachu remains outside.
8. Persistent follower behavior begins.

## 7.2 Integration Rule

Treat these as separate responsibilities:

**Cinematic Pikachu emergence**  
→ **persistent follower activation**

The generic follower subsystem must not erase or visually break the cinematic.

---

# 8. Pillar 2 — Partner Pikachu Narrative State

Partner Pikachu is both a gameplay system and a character.

The Yellow Unique Specification concerns its **narrative behavior**, not the entire follower engine.

---

# 9. Partner Happiness

Partner Pikachu requires persistent happiness/friendship state.

This state must be readable by Yellow-specific story events.

The primary mandatory consumer is Melanie's Bulbasaur event.

## 9.1 Required Rule

Partner Pikachu happiness MUST NOT be faked using:

- badge count;
- story checkpoint;
- generic Pikachu species check;
- ordinary party friendship from an unrelated Pikachu.

The check must resolve the canonical Partner Pikachu.

---

# 10. Evolution Refusal

Partner Pikachu must remain unevolved.

When a Thunder Stone or equivalent evolution attempt targets Partner Pikachu, the game should visibly communicate refusal rather than silently failing.

Ordinary Pikachu are excluded from this restriction.

They remain normal Pokémon and may evolve according to Golden Yellow's normal evolution rules.

---

# 11. Required Partner Pikachu Reaction Events

All four of the following scripted reactions are **mandatory Yellow narrative events**.

They are not optional polish.

Their visible story outcome is canonical; their underlying implementation may use Golden Yellow's modern follower/emotion architecture.

## 11.1 Pewter Pokémon Center — Jigglypuff

Partner Pikachu reacts to Jigglypuff's singing and falls asleep.

**Classification:** Required Yellow character event.  
**Required:** Yes.

## 11.2 Bill's House

Partner Pikachu participates emotionally in Bill's transformation scene.

Required narrative beats:

- Pikachu reacts to transformed Bill;
- Pikachu observes the Cell Separator event;
- Pikachu reacts when Bill returns to normal.

**Classification:** Required Yellow character event.

**Integration:** Retain polished FRLG Bill choreography where useful and layer Yellow Partner reactions into it.

## 11.3 Vermilion Pokémon Fan Club

Partner Pikachu reacts strongly to the Fan Club Pokémon interaction.

Required visible behavior:

- the canonical Partner Pikachu is identified;
- Pikachu displays the special emotional reaction;
- Pikachu temporarily participates in scene-specific choreography;
- normal following resumes safely afterward.

**Classification:** Required Yellow character event.

**Integration:** Use Golden Yellow's follower system rather than copying Yellow's original low-level movement implementation literally.

## 11.4 Pokémon Tower

Partner Pikachu visibly shows fear or unease within the Pokémon Tower narrative context.

**Classification:** Required Yellow character event.

The exact animation and presentation may be modernized, but the emotional reaction must survive.

---

# 12. Partner Reaction Scope Boundary

This document preserves authored Yellow story reactions.

It does NOT attempt to define every possible generic Pikachu happiness face or incidental animation.

Generic follower/emotion behavior belongs in the Partner Pikachu/follower system specification.

---

# 13. Pillar 3 — Rival & Eevee Progression

The Rival's canonical Pokémon is Eevee.

FireRed's starter-counter architecture must not control Yellow-era Rival progression.

---

# 14. Rival Eevee State Machine

The canonical Golden Yellow abstraction is:

`VAR_RIVAL_EEVEE_STATE`

with three persistent states:

- `VAPOREON`
- `FLAREON`
- `JOLTEON`

## 14.1 Exact State Logic

### Oak Lab loss

`VAPOREON`

This is final.

The later optional Route 22 battle does not promote this path.

### Oak Lab win

initially:

`FLAREON`

### Oak Lab win + Route 22 win

promote:

`FLAREON → JOLTEON`

### Oak Lab win + Route 22 loss

remain:

`FLAREON`

### Oak Lab win + Route 22 skipped

remain:

`FLAREON`

---

# 15. Rival Battle #2 — Route 22

**Status:** Optional early battle.

Team:

- Spearow — Lv. 9
- Eevee — Lv. 8

## State Effect

Only if current state is the Lab-win/Flareon path:

**player victory → Jolteon**

A loss does not change it.

Vaporeon path is unaffected.

---

# 16. Rival Battle #3 — Cerulean City

Team:

- Spearow — Lv. 18
- Sandshrew — Lv. 15
- Rattata — Lv. 15
- Eevee — Lv. 17

Eevee remains unevolved.

---

# 17. Rival Battle #4 — S.S. Anne

Team:

- Spearow — Lv. 19
- Rattata — Lv. 16
- Sandshrew — Lv. 18
- Eevee — Lv. 20

Eevee remains unevolved.

---

# 18. Rival Battle #5 — Pokémon Tower 2F

Eevee remains unevolved, but the supporting roster now reflects `VAR_RIVAL_EEVEE_STATE`.

## Jolteon Path

- Fearow — 25
- Shellder — 23
- Vulpix — 22
- Sandshrew — 20
- Eevee — 25

## Flareon Path

- Fearow — 25
- Magnemite — 23
- Shellder — 22
- Sandshrew — 20
- Eevee — 25

## Vaporeon Path

- Fearow — 25
- Vulpix — 23
- Magnemite — 22
- Sandshrew — 20
- Eevee — 25

### Important State Rule

`VAR_RIVAL_EEVEE_STATE` controls more than the final Eevee evolution.

It controls the Rival's broader team construction.

---

# 19. Rival Battle #6 — Silph Co. 7F

The Rival's Eevee is now evolved.

## Jolteon

- Sandslash — 38
- Ninetales — 35
- Cloyster — 37
- Kadabra — 35
- Jolteon — 40

## Flareon

- Sandslash — 38
- Cloyster — 35
- Magneton — 37
- Kadabra — 35
- Flareon — 40

## Vaporeon

- Sandslash — 38
- Magneton — 35
- Ninetales — 37
- Kadabra — 35
- Vaporeon — 40

---

# 20. Rival Battle #7 — Route 22 Pre-League

## Jolteon

- Sandslash — 47
- Exeggcute — 45
- Ninetales — 45
- Cloyster — 47
- Kadabra — 50
- Jolteon — 53

## Flareon

- Sandslash — 47
- Exeggcute — 45
- Cloyster — 45
- Magneton — 47
- Kadabra — 50
- Flareon — 53

## Vaporeon

- Sandslash — 47
- Exeggcute — 45
- Magneton — 45
- Ninetales — 47
- Kadabra — 50
- Vaporeon — 53

---

# 21. Rival Battle #8 — Champion

The same branch established by the first two Rival battles survives through the final Yellow Champion encounter.

## Jolteon Champion

- Sandslash — 61
- Alakazam — 59
- Exeggutor — 61
- Cloyster — 61
- Ninetales — 63
- Jolteon — 65

## Flareon Champion

- Sandslash — 61
- Alakazam — 59
- Exeggutor — 61
- Magneton — 61
- Cloyster — 63
- Flareon — 65

## Vaporeon Champion

- Sandslash — 61
- Alakazam — 59
- Exeggutor — 61
- Ninetales — 61
- Magneton — 63
- Vaporeon — 65

---

# 22. Rival Battle Modernization Rule

Species composition and Eevee-state branching are Yellow canon.

Golden Yellow MAY modernize:

- movesets;
- abilities;
- natures;
- IV/EV assumptions;
- AI;
- held items where appropriate;
- Physical/Special split interactions.

Modernization must not destroy the recognizable Yellow team progression.

---

# 23. Pillar 4 — Yellow Starter Gift Storyline

Bulbasaur, Charmander, and Squirtle are not selectable starters.

They become obtainable through separate Yellow/anime-inspired story events.

---

# 24. Melanie's Bulbasaur — Cerulean City

## Yellow Canon

Location:

**Melanie's house, Cerulean City**

Gift:

**Bulbasaur — Lv. 10**

Requirement:

**Partner Pikachu happiness ≥ 147**

## Required Conditions

- resolve the canonical Partner Pikachu;
- read its happiness state;
- require a threshold of 147 or greater;
- player accepts;
- gift succeeds;
- one-time state is recorded.

## Modern Integration

Modern storage handling is permitted.

If the player's party is full, Golden Yellow may use its normal safe gift/storage workflow rather than reproducing Gen I limitations.

---

# 25. Damian's Charmander — Route 24

## Yellow Canon

Location:

**Route 24**

Gift:

**Charmander — Lv. 10**

## Conditions

No Yellow-script requirement for:

- badge count;
- Partner happiness;
- Rival result

should be invented.

The player needs to reach the NPC and accept the gift.

## One-Time Rule

After successful receipt, the event is permanently marked complete.

---

# 26. Officer Jenny's Squirtle — Vermilion City

## Yellow Canon

NPC:

**Officer Jenny**

Location:

**Vermilion City**

Gift:

**Squirtle — Lv. 10**

Story requirement:

**Lt. Surge must be defeated / Thunder Badge obtained.**

The Officer Jenny identity must be preserved rather than converted into an unrelated generic NPC.

## Modern Integration

Use Golden Yellow's modern:

- gift handling;
- Pokédex registration;
- nickname support;
- party/storage fallback.

---

# 27. Starter Gift Global Rule

Bulbasaur, Charmander, and Squirtle:

- remain ordinary members of their species;
- evolve normally;
- are not Partner Pokémon;
- must not inherit Partner Pikachu restrictions.

---

# 28. Pillar 5 — Jessie, James & Anime Character Events

Jessie and James are canonical Yellow story characters.

They are not optional references or cosmetic trainer replacements.

Each appearance should preserve:

- location;
- narrative placement;
- recognizable approach/choreography;
- dialogue intent;
- battle;
- post-battle disappearance/state;
- Meowth participation.

---

# 29. Jessie & James Encounter #1 — Mt. Moon B2F

Team:

- Ekans — Lv. 14
- Meowth — Lv. 14
- Koffing — Lv. 14

### Narrative Role

This is the player's first encounter with the anime-inspired Rocket duo.

### Required

- Jessie and James both appear;
- Meowth is represented through their battle team;
- encounter remains part of Mt. Moon Rocket progression;
- defeated state prevents repetition.

---

# 30. Jessie & James Encounter #2 — Rocket Hideout B4F

Team:

- Koffing — Lv. 25
- Meowth — Lv. 25
- Ekans — Lv. 25

The encounter remains integrated into Giovanni's Rocket Hideout progression.

---

# 31. Jessie & James Encounter #3 — Pokémon Tower 7F

Team:

- Meowth — Lv. 27
- Arbok — Lv. 27
- Weezing — Lv. 27

Their Pokémon have now evolved.

The battle belongs to the Mr. Fuji rescue progression.

---

# 32. Jessie & James Encounter #4 — Silph Co. 11F

Team:

- Weezing — Lv. 31
- Arbok — Lv. 31
- Meowth — Lv. 31

This is their final Yellow campaign battle.

---

# 33. Jessie & James Modern Battle Rule

Golden Yellow MAY modernize suitable Jessie/James encounters into double battles.

If converted:

- Jessie and James must still read as a coordinated duo;
- the battle must preserve their canonical Pokémon identity;
- the resulting battle must not remove story choreography.

This is a modernization, not a canon rewrite.

---

# 34. Anime Character Identity

## 34.1 Officer Jenny

Officer Jenny should be retained where Yellow specifically uses that anime identity, most importantly the Vermilion Squirtle event.

## 34.2 Nurse Joy / Chansey

Yellow's anime presentation of Pokémon Center staff may be retained or expanded where compatible with the final GBA presentation.

This is a presentation identity rather than a major progression system.

It should not receive disproportionate implementation priority over story-critical Yellow events.

---

# 35. Pillar 6 — Yellow-Specific Boss Progression

Yellow's Gym roster identity should be preserved even where the reason for the change was balance rather than explicit anime adaptation.

The final Golden Yellow battles may be mechanically modernized, but the recognizable Yellow species composition should remain the baseline.

---

# 36. Brock — Pewter Gym

- Geodude — Lv. 10
- Onix — Lv. 12

### Yellow Identity

Lower-level Brock opening appropriate to Yellow's Pikachu-centered campaign.

---

# 37. Misty — Cerulean Gym

- Staryu — Lv. 18
- Starmie — Lv. 21

### Classification

Yellow boss baseline.

Not a major anime overhaul requiring separate story logic.

---

# 38. Lt. Surge — Vermilion Gym

- Raichu — Lv. 28

### Yellow/Anime Identity

Surge uses a **solo Raichu**.

This is one of Yellow's defining anime-inspired boss changes.

### Integration

Modernize Raichu itself rather than automatically adding unrelated team members merely to increase difficulty.

Possible later balance tools include:

- better moves;
- improved AI;
- Ability;
- held item where appropriate.

---

# 39. Erika — Celadon Gym

- Tangela — Lv. 30
- Weepinbell — Lv. 32
- Gloom — Lv. 32

### Classification

Yellow boss baseline.

---

# 40. Koga — Fuchsia Gym

- Venonat — Lv. 44
- Venonat — Lv. 46
- Venonat — Lv. 48
- Venomoth — Lv. 50

### Yellow/Anime Identity

Preserve the escalating **Venonat → Venomoth wall**.

Do not replace it with FireRed's generic poison roster.

---

# 41. Sabrina — Saffron Gym

- Abra — Lv. 50
- Kadabra — Lv. 50
- Alakazam — Lv. 50

### Yellow/Anime Identity

The team represents the complete Abra evolutionary line.

### Integration

Abra's modern battle usability may be improved through appropriate modern moveset design while retaining species identity.

---

# 42. Blaine — Cinnabar Gym

- Ninetales — Lv. 48
- Rapidash — Lv. 50
- Arcanine — Lv. 54

### Classification

Yellow boss baseline.

---

# 43. Giovanni — Persian Progression

Giovanni's anime-associated Persian identity is not limited to Viridian Gym.

Persian appears in all three Yellow Giovanni teams.

## 43.1 Rocket Hideout B4F

- Onix — Lv. 25
- Rhyhorn — Lv. 24
- Persian — Lv. 29

## 43.2 Silph Co. 11F

- Nidorino — Lv. 37
- Persian — Lv. 35
- Rhyhorn — Lv. 37
- Nidoqueen — Lv. 41

## 43.3 Viridian Gym

- Dugtrio — Lv. 50
- Persian — Lv. 53
- Nidoqueen — Lv. 53
- Nidoking — Lv. 55
- Rhydon — Lv. 55

### Required Identity

Persian should persist across Giovanni's Golden Yellow progression.

Do not import FireRed's Kangaskhan-based Giovanni identity over Yellow canon.

---

# 44. Boss Modernization Rule

Species composition is Yellow canon.

Golden Yellow MAY change:

- movesets;
- abilities;
- natures;
- AI;
- held items;
- IV/EV tuning;
- battle scripting;
- Physical/Special interactions.

Changes should strengthen the Yellow roster rather than replacing it by default.

---

# 45. Pillar 7 — Yellow-Specific Progression & Modern Integration

This section exists specifically to prevent Yellow fidelity work from accidentally deleting good later-generation systems.

---

# 46. Viridian Old Man + Teachy TV

This is the primary integration example.

## Yellow Canon

The Viridian Old Man participates in early Viridian progression and tutorial context.

## Modern Upgrade

**Teachy TV MUST remain available.**

## Integration Rule

Do not directly replace FRLG's modern tutorial with Yellow's original Game Boy catching tutorial.

Instead:

- preserve a Yellow-compatible Old Man role;
- retain Teachy TV;
- revise dialogue/state as necessary so the combined sequence feels intentional.

The player should not receive two redundant mandatory catching tutorials unless later design specifically calls for it.

---

# 47. FireRed Map Geometry

Yellow narrative placement does not require literal Game Boy map coordinates.

Use FRLG/GBA map geometry where practical.

Preserve:

- event order;
- relative character position;
- timing;
- approach direction;
- visible behavior;
- narrative destination.

Translate behavior, not raw coordinate numbers.

---

# 48. Running Shoes

Running Shoes are a Golden Yellow modernization feature.

They remain even though Yellow did not contain them.

No Yellow fidelity pass may remove them solely for historical accuracy.

---

# 49. Modern Battle Mechanics

All Yellow battles operate within Golden Yellow's modern battle architecture.

Retain:

- Abilities;
- Natures;
- Gen III IV/EV structure;
- held items;
- Physical/Special split;
- modernized move behavior;
- modern AI where appropriate.

Yellow trainer species composition and narrative identity remain authoritative.

---

# 50. Modern Gift Handling

Yellow gift events retain their original:

- NPC;
- location;
- progression condition;
- species;
- level;
- one-time nature.

The engine MAY use modern handling for:

- full party;
- storage;
- nickname prompts;
- Pokédex updates;
- special Pokémon metadata.

---

# 51. Modern Follower Engine

Partner Pikachu reaction scenes should use one persistent Golden Yellow follower architecture.

Do not recreate a separate Yellow-only follower subsystem for each map.

Scene scripts may temporarily:

- suspend following;
- position Pikachu;
- animate Pikachu;
- restore following.

Persistent state remains engine-owned.

---

# 52. Moltres Placement

Golden Yellow uses Yellow-era canonical Moltres placement:

**Victory Road**

Do not inherit FireRed's Mt. Ember Moltres merely because the FRLG content exists.

Detailed legendary battle implementation may live in a separate legendary specification.

---

# 53. Map-by-Map Yellow Override Index

The following maps/areas contain Yellow-specific content that this specification requires or protects.

## Pallet Town

- Oak interruption
- special Pikachu encounter
- Oak/Pikachu opening choreography

## Oak's Lab

- Eevee theft
- Partner Pikachu gift
- Rival Battle #1
- Eevee branch initialization
- Pikachu emergence/follower origin

## Viridian City

- Yellow-compatible early Old Man state
- Teachy TV retained as modern integration

## Route 22

- Rival Battle #2
- Jolteon branch trigger
- Rival Battle #7 pre-League

## Pewter City / Pokémon Center

- Brock Yellow team
- required Pikachu/Jigglypuff reaction

## Mt. Moon B2F

- Jessie & James #1

## Cerulean City

- Rival Battle #3
- Melanie/Bulbasaur
- Misty Yellow team

## Route 24

- Charmander gift

## Bill's House

- required Partner Pikachu transformation-scene reactions

## S.S. Anne

- Rival Battle #4

## Vermilion Pokémon Fan Club

- required Partner Pikachu special reaction

## Vermilion Gym

- Surge solo Raichu

## Vermilion City

- Officer Jenny Squirtle gift

## Rocket Hideout B4F

- Jessie & James #2
- Giovanni Persian team #1

## Celadon Gym

- Erika Yellow team

## Pokémon Tower

- Rival Battle #5
- required Partner Pikachu fear reaction

## Pokémon Tower 7F

- Jessie & James #3

## Fuchsia Gym

- Koga Venonat/Venomoth team

## Silph Co. 7F

- Rival Battle #6

## Silph Co. 11F

- Jessie & James #4
- Giovanni Persian team #2

## Saffron Gym

- Sabrina Abra-line team

## Cinnabar Gym

- Blaine Yellow team

## Viridian Gym

- Giovanni Persian team #3

## Victory Road

- Yellow-canonical Moltres placement

## Champion's Room

- Rival Battle #8
- final Eeveelution branch payoff

---

# 54. Explicit Non-Scope

This reference is not a complete "Pokémon Yellow versus FireRed differences" database.

The following should generally be documented elsewhere.

## Encounter Specification

- wild encounter tables;
- version-exclusive removal;
- Gen I/II distribution;
- catch rates.

## Trainer/Balance Specification

- every ordinary trainer;
- minor level differences;
- rematch tiers;
- moveset modernization.

## Item Specification

- ordinary item locations;
- TM distribution;
- shop inventory.

## Map Specification

- tile layouts;
- graphics;
- collision changes;
- environmental redesign.

## Mechanics Specification

- generic follower implementation;
- happiness formula;
- evolution engine;
- full battle engine behavior.

Only Yellow-specific dependencies on those systems belong here.

---

# 55. Preservation Test

For every Kanto implementation pass, developers should ask:

> **If this event were implemented exactly as FireRed normally handles it, would a player lose something that makes this campaign specifically Pokémon Yellow?**

If yes:

- consult this specification;
- preserve the Yellow identity;
- integrate the modern system around it.

If no:

- normal modern FRLG/expansion behavior may remain.

---

# 56. Final Yellow Campaign Identity

A successful Golden Yellow Kanto campaign must make the following progression unmistakable:

**Oak catches Pikachu**  
→ **Rival takes Eevee**  
→ **player receives Partner Pikachu**  
→ **Pikachu refuses its Poké Ball and follows**  
→ **Rival's Eevee future is shaped by the first two battles**  
→ **player builds trust with Partner Pikachu**  
→ **Bulbasaur/Charmander/Squirtle are obtained through Yellow story gifts**  
→ **Jessie and James repeatedly interfere with the player's Rocket progression**  
→ **Gym Leaders and Giovanni retain their Yellow identities**  
→ **Rival's Eevee evolves according to the early branch**  
→ **the same branch determines the Champion team**

All of this should occur inside a modern GBA game that retains compatible FireRed/Gen III/Golden Yellow improvements instead of reverting to Game Boy-era limitations.

---

# 57. Governing Summary

Pokémon Yellow determines:

- **what happens;**
- **who participates;**
- **which Yellow-specific conditions matter;**
- **which Rival state persists;**
- **which anime-inspired events define the campaign.**

FireRed, Emerald, `pokeemerald-expansion`, and Golden Yellow may determine:

- **how those events are presented on GBA hardware;**
- **which modern systems support them;**
- **how quality-of-life features are retained;**
- **how battles are mechanically modernized.**

The objective is neither a FireRed story with Yellow references nor a literal Pokémon Yellow port.

The objective is:

> **Pokémon Yellow rebuilt as if it were a modern GBA-era Pokémon adventure.**

---

# 58. Source References

## Primary Pokémon Yellow technical source

- `pret/pokeyellow`
  - `scripts/PalletTown.asm`
  - `scripts/OaksLab.asm`
  - `scripts/CeruleanMelaniesHouse.asm`
  - `scripts/Route24.asm`
  - `scripts/VermilionCity.asm`
  - `scripts/PokemonFanClub.asm`
  - Yellow map scripts for Jessie/James encounters and Partner Pikachu reactions
  - `data/trainers/parties.asm`

Repository: `https://github.com/pret/pokeyellow`

## Modern presentation/system reference

- `pret/pokefirered`
  - FRLG map geometry
  - movement/event architecture
  - Viridian Teachy TV integration
  - GBA presentation patterns

Repository: `https://github.com/pret/pokefirered`

## Golden Yellow project authorities

- `Pokemon_Golden_Yellow_Project_Master.md`
- `Pokemon_Golden_Yellow_Hard_Rules.md`

These project documents remain authoritative for Golden Yellow-specific modernization, source hierarchy, Partner Pikachu architecture, and development governance.
