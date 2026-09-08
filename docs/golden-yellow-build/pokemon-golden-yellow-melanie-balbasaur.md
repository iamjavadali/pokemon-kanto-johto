# Pokémon Golden Yellow — Melanie and Bulbasaur Implementation Notes

**Current status:** IMPLEMENTED AND MANUALLY ACCEPTED  
**Final accepted source tip:** `2f478f5caca2a44d4915ddacc87aac100ec8adff`  
**Acceptance date:** September 8, 2026  
**Dedicated record:** `Pokemon_Golden_Yellow_Melanie_Bulbasaur_Acceptance_Record.md`

> This document began as a research and implementation plan. The accepted implementation now supersedes future-tense wording below where they conflict. Final placement is Melanie `(8,2)` with Bulbasaur `(7,2)`; the gift checkpoint starts outside the house at Cerulean `(15,18)` and initializes Partner friendship/mood to `200/200`.

---

What I confirmed

The house in your screenshot is CeruleanCity_House3_Frlg. The Cerulean exterior warp at (15,17) leads to that house, while the Pokémon Center is the nearby warp at (22,19), matching your screenshot. We therefore do not need to change the Cerulean exterior map or create a new building.

Inside, the current map has only two NPC objects:

Existing object	Current position	Current purpose
Old man	(2,2)	Jynx in-game trade
Old woman	(7,5)	Talks about her husband's Pokémon trading

The three-wide exit remains at (3,7), (4,7), (5,7). There are currently no coordinate triggers or background events in the house.

Their scripts are also self-contained. The old man owns the existing Jynx trade and the old woman only has her normal dialogue. We can leave both scripts untouched and append Melanie/Bulbasaur logic below them.

Yes — we already have an NPC sprite for Melanie

We do not need a custom girl sprite for this phase.

The project already contains:

OBJ_EVENT_GFX_LASS_FRLG

It is a standard FRLG female NPC object and is already used throughout the project, including existing Cerulean content.

I recommend using OBJ_EVENT_GFX_LASS_FRLG for Melanie. It fits the Yellow caretaker/girl role better than introducing a new custom overworld sprite right now.

There is also OBJ_EVENT_GFX_LITTLE_GIRL_FRLG, but Melanie should read more like a young caretaker/trainer than a small child, so Lass is the stronger choice.

Bulbasaur is even easier

We also do not need a new Bulbasaur overworld asset.

This build has overworld Pokémon object support enabled:

OW_POKEMON_OBJECT_EVENTS TRUE

and specifically supports using:

OBJ_EVENT_GFX_SPECIES(BULBASAUR)

for an NPC-style overworld Bulbasaur.

So the two new map objects can be native existing assets:

Melanie
OBJ_EVENT_GFX_LASS_FRLG

Bulbasaur
OBJ_EVENT_GFX_SPECIES(BULBASAUR)

That is preferable to making a fake NPC sprite or importing graphics.

Proposed Bulbasaur Scene Plan

This can be a deliberately small Yellow integration pass.

File	Planned change
data/maps/CeruleanCity_House3_Frlg/map.json	Add Melanie object + Bulbasaur object. Keep old man, old woman, warps, layout unchanged.
data/maps/CeruleanCity_House3_Frlg/scripts.inc	Add Melanie interaction, friendship gate, Bulbasaur interaction, gift sequence, and post-gift dialogue.
include/constants/flags_frlg.h or the established Golden Yellow state allocation area	Add/allocate a persistent named state for the Bulbasaur gift if an appropriate existing one is not already reserved.
Existing Partner helper/special	Reuse, not rewrite, the canonical Partner resolver/friendship reader already powering the Partner/Bond systems.
Graphics files	No change expected.
CeruleanCity_Frlg/map.json	No change required. Existing house entrance is already correct.
1. Add Melanie without touching the old couple

object_events currently contains two entries. We simply append Melanie as object #3.

She should use something like:

OBJ_EVENT_GFX_LASS_FRLG

with a fixed or walk-in-place facing direction rather than wandering around the house.

I would make her essentially stationary because she owns a story gift event. That prevents her from wandering into the table, doorway, Bulbasaur, Partner Pikachu, or the existing couple.

2. Add a real overworld Bulbasaur

Append object #4 using:

OBJ_EVENT_GFX_SPECIES(BULBASAUR)

Bulbasaur should also remain essentially stationary, positioned beside Melanie.

It should have its own persistent hide flag, so:

before adoption: visible
after successful adoption: hidden

Melanie should remain in the house after the gift and switch to post-adoption dialogue.

That gives us the same clean object lifecycle we have been using elsewhere.

3. Preserve the exact Yellow friendship requirement

Your Yellow specification already locks this rule:

canonical Partner Pikachu friendship ≥ 147

and explicitly forbids using an ordinary Pikachu, badge count, checkpoint, or substitute value.

So Melanie's decision tree should be:

Gift already received?
→ post-gift dialogue

Otherwise:

Canonical Partner Pikachu available?
→ if no, Melanie does not offer Bulbasaur

Partner friendship < 147?
→ Melanie says Bulbasaur needs a trainer it can trust / that the player and Pikachu should become closer

Partner friendship ≥ 147?
→ Melanie recognizes how well Pikachu trusts the player
→ offers Bulbasaur

This is where we should use the existing Golden Yellow canonical Partner resolver rather than adding another SPECIES_PIKACHU_STARTER party scan specifically for Melanie. Before implementation I would inspect the exact helper currently used by the Bond/P5 Partner logic and call that same path.

4. Gift flow should reuse our Charmander precedent

I inspected the accepted Route 24 Charmander implementation because it solves almost the same technical problem.

It already handles:

givemon SPECIES_CHARMANDER, 10;
success into party;
success into PC;
no-storage failure;
optional nickname;
only committing the story state after successful acquisition;
hiding/removing the overworld Pokémon after success.

That is exactly the pattern Bulbasaur should reuse.

For Bulbasaur:

givemon SPECIES_BULBASAUR, 10

Then branch on the result:

party success
→ mark adoption complete
→ hide/remove Bulbasaur
→ fanfare / obtained text
→ nickname option

PC success
→ same state commit
→ hide/remove Bulbasaur
→ obtained text
→ nickname option
→ PC transfer message

no storage
→ do not consume the event
→ do not hide Bulbasaur
→ player can return and try again

This is safer than simply setting the flag before givemon.

5. Bulbasaur itself should be interactable

Before adoption, pressing A on Bulbasaur should not silently do nothing.

A simple Yellow-style interaction is enough:

Bulbasaur faces the player
→ Bulbasaur cry
→ short text such as it watching the player cautiously / seeming interested

I would not allow talking directly to Bulbasaur to bypass Melanie's friendship gate. Melanie remains the owner of the gift.

After adoption the overworld Bulbasaur object disappears, so there is no post-gift Bulbasaur script to maintain.

Placement inside the current house

I agree with your requirement: do not remove or relocate the old man or old woman.

I also would not alter the furniture or house layout.

The current map is small but has enough usable floor space to add two stationary objects. Because the old man is (2,2), the old woman is (7,5), and the exit occupies the bottom center (3–5,7), Melanie and Bulbasaur should be placed together in an unused section of floor that does not obstruct:

the three-wide doorway;
the old man trade interaction;
the old woman's tile;
the circulation around the table;
Partner Pikachu's follower positioning.

From your screenshot, the upper-right portion of the room looks like the most natural story composition: Melanie and Bulbasaur together along that side of the room, separated from the old man's trade station and the old woman at the table.

I would still confirm the exact passable tile coordinates from the map layout before committing positions rather than guessing coordinates from the screenshot.

One thing I would not do

I would not turn this into a large cinematic like Route 24 Charmander.

The Yellow Bulbasaur scene is fundamentally a trust-gated gift interaction, so we do not need:

new coordinate triggers;
forced player movement;
follower parking;
room exit guards;
map geometry changes;
a new Partner reaction subsystem.

That keeps the pass narrow and much lower risk.

A modest acknowledgement of Partner Pikachu could be added during the successful friendship check—e.g. Melanie explicitly recognizes how attached Pikachu is to the player—but the friendship value itself should drive eligibility, not a staged Partner animation.

Proposed state flow
ENTER HOUSE
   |
   +-- Old man -> existing Jynx trade (unchanged)
   |
   +-- Old woman -> existing dialogue (unchanged)
   |
   +-- Bulbasaur -> cry / observation text
   |
   +-- Melanie
          |
          +-- gift already completed
          |      -> post-gift dialogue
          |
          +-- canonical Partner unavailable
          |      -> not-ready dialogue
          |
          +-- Partner friendship 0–146
          |      -> trust requirement dialogue
          |
          +-- Partner friendship 147–255
                 -> offer Lv.10 Bulbasaur
                        |
                        +-- NO
                        |    -> Bulbasaur remains
                        |
                        +-- YES
                             -> givemon
                                  |
                                  +-- party success
                                  |    -> commit gift
                                  |
                                  +-- PC success
                                  |    -> commit gift
                                  |
                                  +-- no room/storage
                                       -> no state change
Manual acceptance boundary I would use

The implementation should not be considered accepted until we test: the old couple still function; Melanie and Bulbasaur appear before adoption; ordinary Pikachu cannot satisfy the check; Partner friendship 146 fails and exactly 147 succeeds; declining leaves the event intact; party and PC gift paths both work; no-storage does not consume the gift; Bulbasaur disappears only after success; Melanie remains afterward; re-entering the house reconstructs correctly; and Partner Pikachu/follower behavior is unaffected.

So yes: we already have both the Melanie NPC graphics and Bulbasaur overworld infrastructure. No new art is necessary. This looks like a clean, mostly two-file map/script implementation plus one persistent state allocation.
