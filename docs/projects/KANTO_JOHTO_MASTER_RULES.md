# Kanto + Johto Project Rules

## Project goal
Build one custom GBA Pokemon adventure that unifies Kanto and Johto into a single continuous game and allows a player to obtain and register every Generation I and Generation II Pokemon (#001-#251) in one save without requiring another version or mandatory trading.

## Canonical development base
- Engine/toolkit: `pokeemerald-expansion`.
- Primary build target: FireRed (`make firered`).
- Visual direction: FireRed/LeafGreen-era GBA presentation.
- Kanto source inspiration: FireRed/LeafGreen plus selected Pokemon Yellow/anime-style events.
- The opening starter-acquisition sequence is a special case: the actual Pokémon Yellow game is the canonical source of truth and its ordered story beats must not be replaced by anime recollections or approximations.
- Johto source inspiration: Gold/Silver/Crystal, with selective HeartGold/SoulSilver ideas used as design reference only.
- Do not attempt to binary-merge commercial ROMs.

## Player identity and naming
- The protagonist is custom and player-named; the player is not Ash, Red, Gold, Ethan, or another canon protagonist.
- Preserve gender/name selection and show the selected protagonist presentation clearly during selection.
- Protagonist preset-name choices must store the exact displayed selection; they must never silently randomize to another preset.
- A `NEW NAME` choice must open a clean custom naming screen.
- Major rival-style identities that are meant to belong to the player's playthrough must be player-named.
- Kanto rival is player-named.
- Johto rival is player-named when introduced; do not automatically call that character Silver.
- Canonical world NPCs such as Professor Oak, Professor Elm, Gym Leaders, Giovanni, Jessie, and James may retain their established names.

## Opening and starter philosophy
- Pikachu is the player's original partner/starter through the canonical Pokémon Yellow starter-acquisition sequence.
- Pikachu remains a normal usable Pokemon: trainable, boxable, breedable when breeding is available, and evolvable into Raichu by player choice.
- Bulbasaur, Charmander, and Squirtle are acquired later through story quests inspired by their memorable anime acquisition stories rather than handed out as a starter selection.
- Chikorita, Cyndaquil, and Totodile are likewise acquired through Johto story quests rather than a one-of-three permanent choice.
- Story-acquired Pokemon remain normal Pokemon after acquisition. The anime may inspire how they join the player but must not force their later evolution, departure, moves, party position, or usage.

## Starter Pikachu progression contract
- Starter Pikachu begins at level 5 and initially knows only `Thunder Shock` and `Growl`, matching Pokémon Yellow.
- Regular Pikachu uses the Pokémon Yellow species stat profile: HP 35, Attack 55, Defense 30, Speed 90, and Special-equivalent 50. In the Gen III split-stat engine, both Special Attack and Special Defense use 50 for this Yellow-faithful profile.
- Pikachu uses the Medium Fast experience-growth curve.
- Pikachu's level-up learnset must preserve the Pokémon Yellow schedule: Tail Whip at 6, Thunder Wave at 8, Quick Attack at 11, Double Team at 15, Slam at 20, Thunderbolt at 26, Agility at 33, Thunder at 41, and Light Screen at 50.
- Do not add an artificial experience penalty, hidden stat handicap, or starter-only weakening mechanic.
- Selective modern battle/QoL mechanics may remain where the wider project deliberately uses them, but they must not accidentally replace or weaken the Yellow starter progression contract above.

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
- Do not substitute an anime-inspired event for a Pokémon Yellow game event that the project has explicitly locked as canonical.
- Preserve the exploration, Gym, badge, League, Team Rocket, legendary, and regional progression expected from the games while integrating the combined story into one continuity.
- Planned macro structure: Kanto -> Kanto League -> Sevii transition/expansion -> Johto -> Johto/combined League progression -> late-game resolution -> legendary quests/Mt. Silver/endgame.
- Target 16 total Kanto + Johto badges.

## Prototype v0.1 scope
Prototype v0.1 is an architecture and build-validation milestone, not the full game. Its intended incremental scope is:
1. Reproducible FireRed build in GitHub Actions.
2. Downloadable development ROM artifact for manual testing.
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
- Do not commit generated `.gba` ROMs to Git; publish test builds as short-retention GitHub Actions artifacts.
- Preserve upstream attribution/licenses/credits and document custom assets and code as they are introduced.

## Development validation workflow
- Draft prototype pull requests are the active implementation/manual-test lane. They must not run the full Emerald + FireRed + LeafGreen + release + test matrix on every push.
- Prototype branch pushes use the dedicated FireRed development workflow as the iteration gate.
- The FireRed development workflow first validates the documentation index, then shallow-fetches `master`, detects C source changes relative to the baseline, and compiles only the affected FireRed C objects as a fast preflight.
- If preflight fails, the full FireRed ROM build must not start. Fix the failure immediately and push a correction.
- If preflight passes, build the FireRed target, verify `pokefirered.gba`, generate a SHA-256 checksum, and upload the ROM as a one-day Actions artifact.
- Build/preflight logs are uploaded as one-day diagnostic artifacts on failure so compiler errors are available without creating permanent diagnostic workflows.
- Superseded prototype builds are cancelled automatically when a newer branch commit is pushed.
- Documentation-only changes and changes isolated to the full CI workflow do not trigger a prototype ROM build.
- The prototype development job has a 20-minute timeout. A normal FireRed build is expected to finish substantially earlier; investigate instead of waiting indefinitely if runtime materially exceeds the established baseline.
- A successful targeted FireRed artifact is enough to begin manual gameplay testing. Do not wait for the complete cross-target regression matrix before giving the player a test ROM.
- Full CI is the candidate/acceptance gate. Mark a draft PR ready for review only when the current gameplay slice has passed manual testing and is ready for full regression; use manual dispatch when a full matrix is needed without changing PR state.
- Full CI also runs on protected upstream-style branch pushes.
- Full CI retains parallel Emerald, FireRed, LeafGreen, release, test, and docs validation so accepted candidates receive broad regression coverage without slowing ordinary iteration.
- Never continue waiting on unrelated CI jobs after the targeted development target has already failed; diagnose and correct the failing head first.

## Prototype v0.1 implementation status

### Gameplay Step 1 — The Beginning

- FRLG new-game protagonist gender selection is retained and enhanced with a live protagonist graphic preview while moving between `BOY` and `GIRL`.
- Protagonist naming offers preset names or `NEW NAME`; preset selections store the exact displayed name and custom naming begins from a clean input buffer.
- FRLG rival naming is retained; the Kanto rival remains player-named.
- The Pallet Town opening follows the Pokémon Yellow game sequence: the player reaches the north Route 1 grass exit, Oak calls out and approaches, warns about the grass, and a dedicated level-5 Pikachu capture battle begins.
- The Oak capture scene uses the existing scripted catch-tutorial battle machinery with an Oak-specific presentation rather than the unrelated Viridian old-man presentation.
- After the capture battle, Oak returns to the field, reacts, explains why the player needs a Pokémon, and physically leads the player to the lab.
- The rival is already waiting in Oak's Lab.
- Only the Eevee Poké Ball is presented as the available starter; the player approaches it, the rival pushes ahead and takes Eevee, and Oak then gives the player the same Pikachu caught outside.
- Starter Pikachu is level 5 with Thunder Shock and Growl only, uses the Yellow stat/growth/level-up progression contract above, and remains a normal trainable/boxable/evolvable owned Pokémon.
- The first rival battle uses level-5 Eevee and dedicated face-to-face positioning.
- After the rival leaves, Pikachu comes back out of its Poké Ball; Oak explains its behavior and the existing lead-party follower system activates at that story beat.

## Canonical Pokémon Yellow opening contract

The Kanto opening must preserve the Pokémon Yellow starter sequence in order:
1. The player leaves home and walks north to the Route 1/tall-grass exit.
2. Oak calls out from behind and physically approaches the player.
3. Oak warns that wild Pokémon live in the tall grass.
4. A dedicated wild level-5 Pikachu encounter/capture battle begins and Oak catches Pikachu.
5. The game returns to the field; Oak reacts, explains the need for a Pokémon, and tells the player to follow him.
6. Oak walks to the laboratory and the player automatically follows.
7. The rival is already waiting inside Oak's Lab.
8. Oak offers the player the single Poké Ball containing Eevee; the rival protests and is told to wait.
9. The player approaches/interacts with the Eevee ball; the rival physically pushes ahead and takes Eevee.
10. Oak confronts the rival, permits him to keep Eevee, and calls the player over.
11. Oak gives the player the same Pikachu caught outside moments earlier.
12. The rival intercepts the player while leaving and challenges the player's Pikachu with level-5 Eevee.
13. After the battle the rival leaves the lab.
14. Pikachu comes back out of its Poké Ball, Oak explains that it dislikes staying inside, and Pikachu begins following the player.

No implementation pass may omit, reorder, or replace these story beats. In particular, do not substitute an anime-only rescue scenario for the actual Pokémon Yellow starter-acquisition sequence.
