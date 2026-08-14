# Kanto + Johto Project Rules

## Project goal
Build one custom GBA Pokemon adventure that unifies Kanto and Johto into a single continuous game and allows a player to obtain and register every Generation I and Generation II Pokemon (#001-#251) in one save without requiring another version or mandatory trading.

## Canonical development base
- Engine/toolkit: `pokeemerald-expansion`.
- Primary build target: FireRed (`make firered`).
- Visual direction: FireRed/LeafGreen-era GBA presentation.
- Kanto source inspiration: FireRed/LeafGreen plus selected Pokemon Yellow/anime-style events.
- Johto source inspiration: Gold/Silver/Crystal, with selective HeartGold/SoulSilver ideas used as design reference only.
- Do not attempt to binary-merge commercial ROMs.

## Player identity and naming
- The protagonist is custom and player-named; the player is not Ash, Red, Gold, Ethan, or another canon protagonist.
- Preserve normal gender/name selection unless deliberately expanded later.
- Major rival-style identities that are meant to belong to the player's playthrough must be player-named.
- Kanto rival is player-named.
- Johto rival is player-named when introduced; do not automatically call that character Silver.
- Canonical world NPCs such as Professor Oak, Professor Elm, Gym Leaders, Giovanni, Jessie, and James may retain their established names.

## Opening and starter philosophy
- Pikachu is the player's original partner/starter through a Yellow/anime-inspired opening.
- Pikachu remains a normal usable Pokemon: trainable, boxable, breedable when breeding is available, and evolvable into Raichu by player choice.
- Bulbasaur, Charmander, and Squirtle are acquired later through story quests inspired by their memorable anime acquisition stories rather than handed out as a starter selection.
- Chikorita, Cyndaquil, and Totodile are likewise acquired through Johto story quests rather than a one-of-three permanent choice.
- Story-acquired Pokemon remain normal Pokemon after acquisition. The anime may inspire how they join the player but must not force their later evolution, departure, moves, party position, or usage.

## Pokedex completion
- Core completion target: #001 Bulbasaur through #251 Celebi.
- Every species must have at least one legitimate acquisition path in a single save.
- No version-exclusive lockout.
- No mandatory external trade.
- Trade evolutions must receive a solo-compatible evolution path while preserving meaningful items/conditions where appropriate.
- Event-only Pokemon such as Mew and Celebi must eventually have in-game quests or another legitimate single-save acquisition method.
- Design should support full Pokedex registration and, where feasible, a 251-species living collection through breeding/repeatable acquisition.

## World and follower rules
- Party slot #1 should be able to act as the player's overworld follower once the general follower system is enabled.
- Pikachu can have special story reactions but is not permanently forced as the follower after the opening.
- Jessie and James travel with an overworld Meowth where appropriate; Meowth is treated as a story character and is not a normal capture target.
- Populate towns, routes, Gyms, farms, shops, and story scenes with appropriate NPC-owned Pokemon in the overworld so people and Pokemon visibly coexist.
- NPC Pokemon may be decorative, interactive, quest-related, or tied to battles/acquisition mechanics.

## Trainer-owned Pokemon acquisition
Trainer Pokemon must not be globally stealable with ordinary Poke Balls. Use explicit eligibility states instead:

- `PROTECTED`: ordinary trainer Pokemon; cannot be captured.
- `CAPTURE_CHALLENGE`: trainer voluntarily offers a designated Pokemon as the reward/opportunity for a structured challenge.
- `RESCUABLE`: stolen/abused/illegitimately held Pokemon that can be rescued from an opponent such as Team Rocket.
- `STORY_CAPTURE`: Pokemon that becomes legally catchable through a story event.

Gym Leaders, rivals, major story partners, and equivalent important trainers should normally have protected teams unless a specific story event explicitly says otherwise.

## Story direction
- Do not reproduce the anime story 1:1.
- Use selected anime-inspired events where they improve acquisition stories, character moments, and the feeling of the world.
- Preserve the exploration, Gym, badge, League, Team Rocket, legendary, and regional progression expected from the games while integrating the combined story into one continuity.
- Planned macro structure: Kanto -> Kanto League -> Sevii transition/expansion -> Johto -> Johto/combined League progression -> late-game resolution -> legendary quests/Mt. Silver/endgame.
- Target 16 total Kanto + Johto badges.

## Prototype v0.1 scope
Prototype v0.1 is an architecture and build-validation milestone, not the full game. Its intended incremental scope is:
1. Reproducible FireRed build in GitHub Actions.
2. Downloadable private ROM artifact for manual testing.
3. Custom protagonist baseline.
4. Player-named Kanto rival baseline.
5. Pikachu opening/starter baseline.
6. Party-slot follower prototype.
7. NPC-with-Pokemon overworld prototype.
8. Jessie + James + Meowth overworld prototype.
9. Trainer Capture Challenge proof of concept.
10. Establish Generation I-II project configuration boundaries without deleting expansion capabilities prematurely.

## Development rules
- Keep `master` as the imported/upstream baseline until a deliberate project release strategy changes it.
- Perform custom work on project branches and review through pull requests.
- Prefer configuration and existing expansion systems before inventing parallel mechanics.
- Keep commits scoped and reversible.
- Every gameplay pass should build successfully before layering the next major system.
- Do not commit generated `.gba` ROMs to Git; publish test builds as private GitHub Actions artifacts.
- Preserve upstream attribution/licenses/credits and document custom assets and code as they are introduced.

## Prototype v0.1 implementation status

### Gameplay Step 1 — The Beginning

- FRLG new-game protagonist gender selection is retained; protagonist naming now offers preset names or a custom typed name, and no protagonist name is auto-selected.
- FRLG rival naming is retained; the Kanto rival remains player-named.
- The original Oak Lab three-starter selection is bypassed.
- Oak gives the player a normal level 5 Pikachu that can be nicknamed, trained, boxed, and evolved normally.
- HGSS-style lead-party follower support is enabled; Pikachu begins following after the first rival battle so the opening battle can stage the two trainers face-to-face.
- Bulbasaur, Charmander, and Squirtle are marked unavailable in Oak's Lab for this opening; their player acquisition remains reserved for later story quests.
- The opening is Yellow-inspired: Oak intercepts the player at the tall grass, brings them to the lab while the three Kanto starter balls are still visible, the player-named rival enters, the three unavailable starter balls are removed, Oak gives Pikachu, and the rival challenges with a level 5 Eevee.
- Starter Pikachu is level 5 with Yellow-faithful opening moves Thunder Shock and Growl only; later moves are learned normally.
- The first rival battle uses dedicated positioning so the rival approaches the player directly instead of reusing the obsolete three-starter movement paths.
