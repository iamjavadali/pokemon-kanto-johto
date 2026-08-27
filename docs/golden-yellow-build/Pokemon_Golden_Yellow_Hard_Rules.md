# Pokémon Golden Yellow — Hard Rules & Development Governance

**Document type:** Living hard-rules and engineering-governance specification  
**Project:** Pokémon Golden Yellow  
**Target platform:** Game Boy Advance  
**Relationship to master document:** Companion authority to `Pokemon_Golden_Yellow_Project_Master.md`  
**Purpose:** Preserve non-negotiable development rules, implementation discipline, source-game translation principles, Git policy, validation policy, and lessons learned during development so they are not lost as the project grows.  
**Last consolidated:** August 27, 2026

---

# 1. Purpose and Authority

This document exists to preserve the hard rules and implementation principles that must remain stable throughout Pokémon Golden Yellow development.

The project uses several sources with deliberately different responsibilities:

1. **Project Master Document** — authoritative for game scope, canon, approved systems, architecture, and long-term direction.
2. **This Hard Rules Document** — authoritative for development discipline, Git policy, implementation constraints, source-translation rules, validation terminology, and durable workflow rules.
3. **Yellow Unique Specification** — authoritative for Yellow-specific Kanto narrative requirements and locked Yellow integration requirements.
4. **FireRed Retention Specification** — authoritative for compatible FireRed/Gen III systems and presentation that Golden Yellow must retain or adapt.
5. **Current Project State** — broad operational snapshot of where development stands and the next active boundary.
6. **Acceptance Log** — canonical cumulative ledger of gameplay progress explicitly accepted by the user.
7. **Dedicated Acceptance Records** — detailed historical records for complex accepted milestones when warranted.
8. **Live GitHub Repository** — authoritative for current implementation state.

These sources serve different purposes and must not be conflated.

If an implementation conflicts with an approved hard rule, the implementation must be corrected unless the rule is explicitly revised.

If this document conflicts with the live repository about what is currently implemented, the repository is authoritative for implementation state, but the conflict must not silently redefine project rules.

---

# 2. Rule Language

The following terms are intentional:

- **MUST / MUST NOT** — mandatory rule.
- **SHOULD / SHOULD NOT** — strong implementation preference; deviation requires a clear technical reason.
- **MAY** — permitted but not required.

Rules are identified with stable IDs so they can be cited in future planning, reviews, and debugging passes.

---

# 3. Git and Branch Rules

## GIT-001 — Sole Development Branch

`prototype/v0.1` is the sole canonical development branch.

All development, fixes, validation commits, gameplay changes, script changes, engine changes, and build-triggering commits MUST occur directly on `prototype/v0.1` unless the user explicitly authorizes an exception.

## GIT-002 — No Temporary Development Branches

Do NOT create temporary builder, scratch, fix, validation, continuation, experiment, Stage1A, or similar branches without explicit user authorization.

## GIT-003 — Resolve Live State Before New Work

Before a new planning, implementation, debugging, code-review, or continuation pass, establish the current `prototype/v0.1` HEAD and inspect the exact files relevant to the task.

Within the same active task, do **not** repeatedly re-fetch files that were already inspected if no intervening repository change has occurred.

Re-check when:

- the branch may have advanced;
- a new build/commit was produced;
- another actor may have changed the affected files;
- the task moves into a different implementation area;
- the prior inspected state is no longer guaranteed current.

Do not rely on remembered commit hashes or previous chat summaries as the implementation baseline.

## GIT-004 — Repository Beats Conversation Memory

When a prior conversation summary, remembered implementation state, or previous plan conflicts with the live repository, the live repository MUST be treated as the implementation source of truth.

## GIT-005 — Continue, Do Not Restart

If equivalent or partial work already exists in the repository, continue from it.

Do not recreate or reapply an earlier planned fix simply because the current session did not perform the original edit.

## GIT-006 — Explicit Branch on Every Write

Every repository write MUST explicitly target `prototype/v0.1`.

Never rely on the default branch for write operations.

## GIT-007 — Temporary Validation Machinery Must Leave No Final Debris

If temporary helper files or workflow files are absolutely necessary on `prototype/v0.1`, they MUST be removed before the pass is considered clean.

The final intended gameplay state should not retain temporary validation machinery unless it is deliberately promoted to permanent project infrastructure.

## GIT-008 — Keep Final History Focused

When practical, temporary mechanical commits used only to apply a large patch or trigger validation SHOULD be squashed or otherwise cleaned so the canonical branch history remains understandable.

Do not rewrite history casually; history cleanup must preserve the intended source tree and branch safety.

## GIT-009 — Never Modify `master` Accidentally

No project development work belongs on the repository default `master` branch unless explicitly authorized.

Before any write, verify the target branch.

## GIT-010 — User-Specified Commit/Build Separation Is Binding

If the user asks to commit changes without building, do not build.

If the user asks for diagnosis only, do not implement.

If the user asks for a build/validation pass after commits are complete, perform that as a separate step.

## GIT-011 — Allowed Repository Branch Inventory

The repository MUST normally contain only these two branches:

- `master` — the imported upstream baseline. Project development MUST NOT occur here unless the user explicitly authorizes it.
- `prototype/v0.1` — the sole canonical Golden Yellow development branch.

Do NOT create or retain `tmp/*`, scratch, builder, validation, continuation, experiment, alternate `prototype/*`, fix, or other development branches unless the user explicitly authorizes a specific exception.

If an unauthorized branch is discovered, branch cleanup MUST occur before new implementation work proceeds. Validation and patch application MUST operate directly on `prototype/v0.1`; temporary workflow files MAY be used only under GIT-007 and MUST be removed during the same pass.

---

# 4. Planning and Implementation Entry Rules

## DEV-001 — Inspect Before Designing

Before proposing a fix, inspect the current code that actually controls the behavior.

For scene work, inspect at minimum:

- map object events;
- coordinate triggers;
- map scripts;
- movement scripts;
- relevant engine helpers;
- relevant source-game scripts when fidelity matters.

## DEV-002 — Audit the Relevant Source Pattern Before Replacing

When a bug occurs in behavior that already exists in FireRed, Yellow, Crystal, or Emerald, first determine how the original game solved the equivalent problem.

Do not invent a new subsystem before checking whether the source game already provides a stable pattern.

## DEV-003 — Prefer Narrow Fixes

Fix the smallest layer that is actually wrong.

Do not change battle logic to solve a map choreography issue.

Do not change the generic follower engine to solve one cinematic scene if the scene can be handled locally.

Do not rewrite a whole sequence when one coordinate or movement count is wrong.

## DEV-004 — Preserve Known-Good Behavior

When fixing one bug, explicitly identify adjacent behavior that is already working and protect it from regression.

A fix MUST NOT casually disturb previously validated behavior such as:

- Yellow Oak capture battle presentation;
- Mom timing once validated;
- Oak approach timing once validated;
- rival Eevee battle logic;
- Yellow dialogue already corrected;
- generic follower behavior outside the affected scene.

## DEV-005 — No Broad Reverts Without Need

Do not restore large portions of FireRed or Yellow wholesale merely because one scene is broken.

Restore only the specific source behavior needed to solve the identified defect.

## DEV-006 — Distinguish Geometry Bugs from Engine Bugs

Before adding C code, determine whether the issue can be solved with:

- trigger coordinates;
- object starting coordinates;
- movement lengths;
- facing;
- map flags;
- script ordering;
- existing event variables.

Only escalate to engine code when map/script architecture cannot represent the required behavior safely.

## DEV-007 — Source-Game Behavior Is Evidence

When original-game behavior is available through source code, gameplay video, screenshots, or decompilation, use it as direct evidence rather than relying on memory.

## DEV-008 — Runtime Evidence Overrides Assumptions

If the code appears correct but manual gameplay shows otherwise, treat the runtime result as evidence that another subsystem is overriding or invalidating the intended behavior.

Do not keep adjusting the same calculation without tracing the downstream lifecycle.

---

# 5. Canon and Source Hierarchy Rules

## CANON-001 — Yellow Owns the Kanto Story

During the initial Kanto campaign, Pokémon Yellow is the narrative authority.

FireRed story logic MUST NOT silently overwrite Yellow event order, dialogue intent, starter structure, rival structure, gift events, Team Rocket structure, or other Yellow-specific canon.

## CANON-002 — FireRed Owns GBA Presentation and Proven GBA Choreography

FireRed/LeafGreen SHOULD be used as the primary reference for:

- GBA map scale;
- map object/event architecture;
- movement scripting patterns;
- UI;
- battle presentation;
- menu behavior;
- native GBA engine structure;
- door and warp choreography;
- object facing and collision conventions.

## CANON-003 — Crystal Owns the Johto Story

Pokémon Crystal is the primary narrative authority for the Johto campaign.

Gold/Silver are supplemental references where Crystal does not fully define behavior.

## CANON-004 — Emerald Is a Systems Reference, Not Narrative Authority

Emerald systems may be reused for facilities, mechanics, scripting infrastructure, and engine improvements without importing Hoenn narrative context.

## CANON-005 — System Source and Story Source May Differ

It is valid for Yellow to define **what happens** while FireRed defines **how the GBA scene is staged**.

This distinction should be made deliberately.

---

# 6. Yellow-to-FRLG Translation Rules

## YFRLG-001 — Translate Yellow Choreography Almost Literally

For Yellow-specific scripted scenes, Yellow's scripted choreography SHOULD be translated almost literally into FRLG coordinates and GBA movement commands.

Preserve:

- event order;
- relative character positions;
- player displacement;
- NPC approach direction;
- facing relationships;
- pauses and reactions;
- when control is taken and restored;
- scene-specific spawn timing.

Only translate the coordinates and movement representation required by the FRLG map geometry.

## YFRLG-002 — Translate Behavior, Not Raw Coordinates

Yellow Game Boy coordinates do not need to match FRLG coordinates numerically.

The invariant to preserve is the visible behavior.

Example:

If Yellow allows the player to walk two tiles into the grass before Oak stops them, the FRLG implementation should reproduce **two meaningful grass steps before interception**, even if the actual FRLG Y coordinate differs.

## YFRLG-003 — Prefer Coordinate Translation Over New Systems

If a Yellow scene can be reproduced by changing:

- a trigger location;
- object coordinates;
- movement counts;
- direction;
- map scene variables;

then do that instead of adding generalized engine code.

## YFRLG-004 — Reuse FRLG's Proven Positional Branching

When FireRed already uses multiple movement paths for left/middle/right or front/back positions, preserve that pattern.

Do not replace carefully authored positional choreography with generic pathfinding unless there is a compelling reason.

## YFRLG-005 — Do Not Import FRLG Story Geometry Blindly

FRLG's original trigger positions are not automatically canonical for Yellow scenes.

Use FRLG movement architecture where valuable, but retain Yellow's intended location and scene geometry.

## YFRLG-006 — Scene-Specific Yellow Logic Belongs Near the Scene

A one-off Yellow cinematic should generally be implemented in its map/event scripts unless it represents a persistent game system.

Do not globalize scene-specific behavior unnecessarily.

---

# 7. Forced Interaction Rules

## SCENE-001 — Forced Interaction Alert Must Match Who Notices

For automatic/forced interactions, use the alert on the entity that is actually reacting to or noticing the other party.

General sequence:

**trigger → appropriate `!` reaction → wait for alert completion → forced movement/turn/dialogue**

Approved examples:

- Pewter pre-Brock Guide redirects/stops the player → `!` over the **player**.
- Running Shoes Aide notices the player and approaches → `!` over the **Aide**.

Do not apply a universal "player always gets the exclamation" rule.

## SCENE-002 — Voluntary A-Button Interaction Does Not Need a Forced Alert

When the player voluntarily presses A to speak to an NPC, do not add a forced `!` unless the source scene explicitly requires one.

Example:

- manually talking to the Pewter Guide → no forced `!`.

## SCENE-003 — No Choreography Before a Required Alert Completes

Once a forced interaction requires an alert, there MUST NOT be dialogue, approach movement, forced player movement, or battle initiation that visually overtakes that alert.

## SCENE-004 — Wait for Alert Completion

The script must wait for the alert movement/emote to complete before continuing the scene.

## SCENE-005 — Facing Is Part of Choreography

When a character speaks to or reacts to another character, facing must be deliberately authored. If the target can occupy more than one side of the speaker, use a dynamic facing mechanism rather than hard-coding one direction.

# 8. Oak Opening Rules

## OAK-001 — Player Must Enter the Grass Before Oak Stops Them

The Yellow opening must allow the player to visibly enter Route 1 grass before Oak's interruption.

The target reference is the original Yellow choreography, including the observed two-tile penetration into the grass lane before Oak stops the player.

## OAK-002 — Oak/Pikachu Encounter Lives in Yellow Geometry

The Oak/Pikachu encounter location must follow Yellow behavior rather than FireRed's original pre-Route-1 Pallet trigger.

## OAK-003 — Preserve the Yellow Capture Scene

The Yellow-specific Oak capture presentation is a protected feature.

Do not replace it with FireRed's ordinary Oak walk-to-lab scene.

## OAK-004 — Route 1 and Pallet Oak Objects Are Separate Map Objects

An object event belongs to its current map.

Do not assume the Route 1 Oak object can physically remain the same object while crossing into Pallet Town.

The Route 1 → Pallet transition must use a deliberate object handoff when necessary.

## OAK-005 — Map Handoff Must Look Continuous

Even when the engine requires an object handoff, the visible sequence should read as one continuous escort.

Avoid:

- flashes;
- visible teleporting;
- duplicated movement segments;
- Oak stopping while the player continues alone;
- Oak reappearing in an unrelated position.

## OAK-006 — Do Not Reuse Movement Paths from the Wrong Starting Geometry

FRLG's original `OakWalkToLabLeft/Right` paths assume FRLG's original starting coordinates.

If the Yellow handoff enters Pallet at different coordinates, use appropriately translated continuation movement rather than replaying a path authored for another start position.

---

# 9. Oak's Lab Rules

## LAB-001 — Use Yellow Event Order

The Oak's Lab starter sequence must follow Yellow's event order:

- Rival wants a Pokémon;
- player moves toward Eevee;
- Rival intercepts and takes Eevee;
- Oak permits Rival to keep Eevee;
- Oak calls player over;
- player receives the previously caught Pikachu;
- Rival later challenges player;
- after the battle Rival exits;
- Pikachu immediately emerges from its Poké Ball;
- Oak explains Pikachu's Poké Ball preference/following behavior.

## LAB-002 — Use FRLG's Lab Geometry Where It Helps

Keep FRLG's map, object architecture, collision layout, door positions, and useful movement conventions unless Yellow fidelity requires a deliberate change.

## LAB-003 — Add Player `!` Before Rival Takes Eevee

When the player selects the Eevee Poké Ball and Rival interrupts, the player must stop and receive the forced-interaction `!` before Rival begins the interception.

## LAB-004 — Rival Must Complete a Clean Approach to Eevee

Rival should not stop one tile short, speak, then take an awkward extra side-step solely because of an artificial split movement script.

Translate the original Yellow choreography into one clean approach appropriate to the FRLG table geometry.

## LAB-005 — Do Not Move the Player Unless They Actually Block Rival

If the player selects Eevee from behind the table and is not occupying Rival's approach/destination path, do not force the player sideways.

If the player does block the approach, use the smallest required displacement.

## LAB-006 — Position Player Directly Relative to Oak

When the script brings the player to Oak, the final tile should be deliberately chosen so Oak can use normal `faceplayer` behavior.

Do not compensate for an incorrect player destination by hardcoding Oak facing.

## LAB-007 — Preserve `faceplayer` Semantics

Oak's normal interaction should use FireRed's existing `faceplayer` behavior.

If Oak faces the wrong direction, first verify the player's final coordinates before adding direction-specific Oak logic.

## LAB-008 — Prefer Coordinate-Specific Movement in Scripted Lab Scenes

The original Yellow and FireRed lab scenes are strongly coordinate-driven.

Use explicit front/back/left/middle/right movement branches where appropriate rather than generalized pathfinding.

---

# 10. Partner Pikachu Rules

## PIKA-001 — Partner Pikachu Is a Distinct Persistent Identity

The canonical Partner Pikachu is not interchangeable with an ordinary Pikachu.

It must remain separately identifiable for restrictions, following, narrative continuity, and future cross-region state.

## PIKA-002 — Partner Pikachu Cannot Evolve or Be Traded

The canonical Partner Pikachu must not evolve or be traded through ordinary gameplay.

## PIKA-003 — Partner Pikachu Is the Cross-Region Continuity Anchor

Partner Pikachu must persist from Yellow-era Kanto through later phases of the project.

## PIKA-004 — Cinematic Spawn and Persistent Following Are Different Responsibilities

The moment Pikachu emerges from its Poké Ball after the Oak Lab Rival battle is a scripted cinematic event.

Normal persistent follower logic begins after that cinematic state is established.

Do not force the generic follower engine to solve the cinematic placement if doing so causes lifecycle conflicts.

## PIKA-005 — Pikachu Must Appear Immediately After Rival Exit

After the first Rival battle:

- Rival exits;
- the player finishes the scripted watch/space-making movement;
- Pikachu appears beside the player **before the player regains ordinary movement**;
- Oak reacts and the Poké Ball refusal dialogue begins.

Pikachu must not wait for the player's next movement input to appear.

## PIKA-006 — Pikachu Must Never Spawn on Top of the Player in This Scene

The scripted Oak Lab emergence must place Pikachu on a valid adjacent tile, not the player's current tile.

## PIKA-007 — Use the Existing Battle-Lane State

The Oak Lab's existing left/middle/right Rival battle trigger state SHOULD be reused to determine deterministic post-battle choreography and the appropriate free adjacent Pikachu tile.

## PIKA-008 — Prefer a Cinematic Pikachu Object for the Emergence Scene

The preferred architecture for the post-Rival emergence is:

1. show/place a normal scripted Pikachu object on the intended adjacent tile;
2. play the emergence/cry/dialogue scene;
3. convert that exact object into the persistent follower once the cinematic placement is complete.

This is closer to original Yellow behavior and avoids follower reappearance logic overwriting cinematic coordinates.

## PIKA-009 — Reuse Existing Follower Conversion Logic

If the follower engine already contains a safe internal path that converts an existing object into a follower while preserving current coordinates/facing, expose/reuse that path rather than building a parallel follower spawning system.

## PIKA-010 — Do Not Globally Alter Follower Behavior for One Cutscene

General follower spawn, warp, door, or reappearance behavior must not be changed solely to fix Oak Lab cinematic positioning unless the underlying generic system is genuinely defective everywhere.

## PIKA-011 — Use the Canonical Partner Identity

Partner-only logic must identify `SPECIES_PIKACHU_STARTER`, not ordinary `SPECIES_PIKACHU`.

Ordinary Pikachu must never trigger Partner-only evolution restrictions, storage restrictions, follower narrative reactions, or story scenes merely because it shares the Pikachu Pokédex identity.

## PIKA-012 — Mandatory Yellow Partner Reaction Set

The Yellow campaign must preserve these Partner-specific reactions:

1. Pewter Pokémon Center Jigglypuff.
2. Bill transformed/restored.
3. Vermilion Pokémon Fan Club.
4. Pokémon Tower fear.

## PIKA-013 — Pokémon Follower and NPC Follower IDs Are Different Systems

For the standard Pokémon follower, use `OBJ_EVENT_ID_FOLLOWER`.

`OBJ_EVENT_ID_NPC_FOLLOWER` belongs to the separate NPC/human follower subsystem and must not be used to animate Partner Pikachu.

## PIKA-014 — Reusable Follower Emotes Belong in Reusable Infrastructure

If a Partner reaction needs an emote/icon that may be useful later, add it as reusable follower/field-effect infrastructure rather than embedding a Pewter-only graphics hack.

---

# 11. Engine Architecture Rules

## ENG-001 — Extend Existing Systems Before Creating New Ones

Prefer extending stable existing infrastructure to creating duplicate systems.

## ENG-002 — Persistent Mechanics Belong in Engine Code

Examples include:

- Partner identity restrictions;
- follower persistence;
- rematch tier logic;
- time systems;
- regional map state;
- alternative trade evolution rules.

## ENG-003 — Scene Choreography Belongs in Map Scripts

One-off NPC movement, dialogue ordering, scene-specific positioning, and local cinematics SHOULD remain map-script-owned unless engine support is genuinely necessary.

## ENG-004 — Avoid Parallel Kanto/Johto Implementations

Fundamental systems such as followers, time, Pokédex, travel state, trainer progression, and save architecture should have one extensible implementation.

## ENG-005 — Avoid Graphics-Coupled Gameplay Hacks

Temporary graphics may be replaced later.

Gameplay logic must not depend on brittle assumptions tied to placeholder artwork.

## ENG-006 — Design State for the Full Game

Kanto flags/variables must be allocated with Sevii, Johto, later Kanto, and endgame in mind.

Do not consume state IDs carelessly in a way that forces later architectural rewrites.

## ENG-007 — Use Safe Scripted Follower Movement for Escort Scenes

When scripted player movement should be copied by the active Pokémon follower, use the project's safe scripted-follower mechanism (`FLAG_SAFE_FOLLOWER_MOVEMENT`) and the standard follower callback path.

Do not author a second duplicated Pikachu movement route merely to mimic the player's scripted path.

## ENG-008 — Safe Follower Mode Must Prevent Collision Recall

A scene explicitly marked for safe scripted follower movement must not put the active Pokémon follower back into its Poké Ball merely because another scripted object temporarily collides with it.

## ENG-009 — Locked Scenes Must Still Allow Intended Follower Copy Movement

If `lock` / `lockall` freezes ordinary non-player object updates, the safe follower path must explicitly preserve the standard follower's ability to consume player copyable movement during the authored scene.

---

# 12. Gameplay and Pokédex Rules

## GAME-001 — All Pokémon #001–#251 Must Be Obtainable Solo

No version exclusives, external trades, external events, or second cartridge may be required for Pokédex completion.

## GAME-002 — Trade Evolutions Require Alternatives

Every trade-based evolution must have a single-player alternative method.

## GAME-003 — Gen II Species Primarily Debut in Johto

Generation II species should not be broadly introduced during the Yellow Kanto or Sevii phases unless a deliberate exception is approved.

## GAME-004 — Gen III Mechanics Are the Mechanical Baseline

The project uses Gen III-style systems plus selected later improvements, including the Physical/Special split.

## GAME-005 — Locked Modern Features Remain Requirements

The following are project requirements:

- Running Shoes;
- Vs. Seeker / scalable rematches;
- stronger Elite Four/Champion rematches;
- Double Battles;
- reusable Move Tutors;
- enhanced #001–#251 Pokédex;
- multi-region Town Map/Pokégear architecture;
- consumable / single-use TMs;
- Emerald-inspired Battle Frontier / Battle Points.

## GAME-006 — TMs Remain Consumable

Do not enable reusable TMs.

Modern TM menu organization or presentation may be retained, but successful TM teaching consumes the TM unless the user explicitly changes this rule later.

## GAME-007 — Exact Yellow Trainer Parity Is Not a Universal Requirement

For non-exclusive trainer battles, if FRLG preserves the correct major Yellow story role and roster identity, exact Yellow level/move/AI parity is not required solely for historical matching.

Yellow-specific roster or narrative differences still override FRLG when they materially define the event.

---

# 13. Cross-Region and Save Rules

## SAVE-001 — One Persistent Save

The game is one continuous adventure, not separate Kanto and Johto saves.

## SAVE-002 — Partner Pikachu Crosses the Era Transition

Partner Pikachu remains with the player through the Kanto → Crystal-era transition.

## SAVE-003 — Do Not Promise Save Compatibility Without Validation

During active development, save-format changes may occur.

Never claim backward save compatibility unless explicitly tested.

## SAVE-004 — Avoid Unnecessary Save Churn

Persistent IDs and save structures should remain stable where practical.

## SAVE-005 — Story State Must Scale Across Regions

Major Kanto, Sevii, Johto, later Kanto, legendary, rival, gift, Frontier, Pokédex, and regional-access state must remain cleanly representable within one save architecture.

---

# 14. Hardware and Performance Rules

## HW-001 — Real GBA Is a Hard Target

A feature is not considered complete if it only works in permissive emulators.

## HW-002 — Respect Real Resource Limits

Continuously account for:

- EWRAM;
- IWRAM;
- VRAM;
- OAM;
- palette memory;
- ROM space;
- save space;
- DMA use;
- sprite/object limits;
- decompression buffers;
- audio resources.

## HW-003 — Avoid Excessive Object-Event Pressure

Follower and cutscene designs must consider map object limits.

Do not add permanent scene objects casually when they can be hidden/reused safely.

## HW-004 — Test Timing-Sensitive Systems on Multiple Environments

At milestone points, validate follower movement, RTC, save behavior, audio, map rendering, and cutscenes across representative emulators and physical hardware when possible.

---

# 15. Debug Checkpoint Rules

## DEBUG-001 — A Checkpoint Is Not Gameplay Implementation

A debug checkpoint proves only that the game can reconstruct and warp into a test state.

The existence of a checkpoint MUST NOT be cited as evidence that the corresponding story event is implemented.

## DEBUG-002 — Reconstruct Gameplay State, Not Merely Inventory

A checkpoint must restore the minimum guaranteed canonical progression state required to test its target:

- mandatory story flags/variables;
- required badges;
- required scene state;
- mandatory key items/unlocks;
- Partner/follower state;
- required party/storage state.

Optional pickups, rods, consumables, TMs, and other non-mandatory items should not be granted merely for convenience unless the target test requires them.

## DEBUG-003 — New Yellow-Specific Work Gets a Pre-Event Checkpoint

When practical, add a debug checkpoint immediately before a new Yellow-specific event or major behavior change so the user can test it directly.

Place campaign checkpoints in chronological order.

## DEBUG-004 — Preserve Existing Checkpoint Numeric IDs

When extending checkpoint enums, append new IDs rather than renumbering established checkpoints unless a deliberate migration is approved.

# 16. Validation Rules

## VAL-001 — Validation Status Terms Are Strict

The project uses these distinct states:

1. Planned / Locked
2. Implemented
3. Build-Validated
4. Automated-Test-Validated
5. Manually Gameplay-Validated
6. Accepted

These labels must not be used interchangeably.

## VAL-002 — Implemented Does Not Mean Working

A committed source change is only **Implemented** until stronger evidence exists.

## VAL-003 — Build Success Does Not Mean Gameplay Success

A compiling ROM is **Build-Validated**, not automatically gameplay-valid.

## VAL-004 — Automated Tests Do Not Replace Manual Gameplay

Automated checks can raise status to **Automated-Test-Validated** where applicable, but visual choreography, softlocks, interaction timing, follower behavior, and cutscenes still require manual play.

## VAL-005 — User Manual Test Controls Manual Validation

Do not mark a scene **Manually Gameplay-Validated** until the user actually tests it and reports that it behaves correctly.

## VAL-006 — Accepted Requires Explicit Acceptance

Do not mark a feature or pass **Accepted** unless the user explicitly accepts it.

A casual build pass or partial manual success is not acceptance.

## VAL-007 — Each Major Build Must Have Provenance

Record, as applicable:

- branch;
- source commit;
- workflow run;
- ROM filename;
- ROM size;
- SHA-256;
- header sanity;
- validation status;
- fresh-save requirement.

## VAL-008 — Pace Long CI Polling Reasonably

When a known GitHub build normally takes several minutes, poll at sensible intervals rather than repeatedly querying fast enough to waste the session or hit timeouts before artifact validation.

---

# 17. Debugging and Regression Rules

## BUG-001 — Diagnose Regression Origin by Commit

When a previously working behavior breaks, compare:

- last known-good commit;
- commit that introduced the architecture change;
- current HEAD.

Identify which exact behavior was removed or changed before adding another patch.

## BUG-002 — Separate Primary Defect from Secondary Symptoms

An unexpected dialogue, wrong interaction, or odd object state may be a consequence of an earlier object-lifecycle failure.

Fix the root cause rather than masking the secondary symptom.

## BUG-003 — Do Not Stack Generalized Fixes on a Misidentified Layer

If several attempts to repair one calculation fail, stop and inspect the lifecycle that occurs after the calculation.

Repeatedly moving an object before another subsystem moves it again is not a valid long-term fix.

## BUG-004 — Preserve Working Portions of Mixed Regressions

If a broad change fixed one part and broke three others, keep the confirmed improvement and surgically restore only the lost behavior.

Do not revert the entire change automatically.

## BUG-005 — Screenshots and Video Are Valid Behavioral Evidence

Use user-provided runtime captures to verify actual coordinates, facing, scene order, timing, and object visibility.

---

# 18. Documentation Rules

## DOC-001 — Master Document Is Direction, Not Changelog

Do not update the master project document for ordinary commits, build artifacts, checksums, or routine bug fixes.

## DOC-002 — This Hard Rules Document Is Also Not an Implementation Log

Do not record every commit or bug here.

Add to this document only when a durable rule, engineering principle, workflow constraint, or recurring implementation standard has been explicitly established.

## DOC-003 — Rules Should Be Stable and Generalizable

A lesson from a specific bug should be written as a reusable principle when possible.

Example:

Bad rule:

> Move Oak to X=12 after commit ABC.

Good rule:

> Map-bound NPCs cannot be assumed to survive connected-map transitions; use explicit object handoff choreography.

## DOC-004 — Do Not Encode Current HEAD as Permanent Authority

Current commit hashes, workflow IDs, and ROM checksums belong in Git/build records, not as the permanent baseline of this document.

## DOC-005 — New Hard Rules Require Deliberate Approval

Do not silently promote an implementation choice into a project-wide hard rule.

When a new durable principle emerges, it should be explicitly recognized before being added here.

## DOC-006 — Every Explicit Acceptance Requires a Documentation Closeout Review

Whenever the user explicitly accepts project-build progress — for example by reporting **PASS**, **FULL PASS**, **accepted**, or an equivalent final approval — the acceptance must be closed out in documentation before the project moves on as though that milestone were settled.

The entire Golden Yellow project-document set MUST be reviewed at acceptance closeout, but each file must be updated according to its own role. Do **not** mechanically edit every file merely to change a date or repeat the same acceptance.

### Acceptance Log — MUST update every acceptance

`Pokemon_Golden_Yellow_Acceptance_Log.md` MUST be updated for every explicit acceptance.

Record, as applicable:

- acceptance date;
- accepted milestone/feature scope;
- source commit;
- workflow/run and ROM provenance when useful;
- explicit manual result;
- important regression invariants;
- link to a dedicated acceptance record when one exists.

If a historical grouped acceptance does not map honestly to one final commit, state that rather than inventing a false single-SHA baseline.

### Current Project State — MUST review; update only when broad state changes

`Pokemon_Golden_Yellow_Current_Project_State.md` MUST be reviewed after every acceptance, but it should be edited only when the acceptance materially changes the broad operational snapshot, such as:

- an active story area becomes broadly complete;
- the next active development boundary moves;
- a major foundation system is established or replaced;
- a blocker is opened or closed in a way that materially affects development;
- the campaign advances into a new phase.

Do not copy commit hashes, workflow IDs, ROM checksums, detailed choreography, or fix-by-fix history into Current Project State merely because a feature was accepted. Current Project State must remain broad and durable.

### Dedicated Acceptance Records — create or update only when warranted

A dedicated acceptance record SHOULD be created or updated when an accepted milestone contains enough branching, choreography, graphics work, engine behavior, or regression-sensitive detail that the project would benefit from a deep historical record.

If a dedicated acceptance record is created:

- link it from the Acceptance Log;
- add it to `docs/SUMMARY.md`;
- keep it scoped to that milestone;
- do not append unrelated future acceptances to it.

An existing dedicated acceptance record should be changed only when its own accepted milestone needs clarification, regression notes, or an explicitly approved redesign.

### Project Master — review, but routine acceptance does not update it

`Pokemon_Golden_Yellow_Project_Master.md` MUST be reviewed for relevance at acceptance closeout but MUST NOT be changed merely because implementation progressed.

Update it only when the accepted work also establishes or changes a project-level decision involving scope, canon, architecture, required systems, campaign-stage definitions, validation policy, or another master-level direction.

### Hard Rules — review, but routine acceptance does not update it

`Pokemon_Golden_Yellow_Hard_Rules.md` MUST be reviewed for lessons that should become durable governance, but routine acceptance history does not belong here.

Update Hard Rules only when the accepted work establishes a durable, generalizable engineering/workflow constraint and that rule has been explicitly approved.

### Yellow Unique Specification — review, update only for requirement changes

`Pokemon_Golden_Yellow_Yellow_Unique_Specification.md` MUST be reviewed when an accepted milestone touches Yellow-specific content.

Update it only if the accepted work changes, corrects, or materially clarifies a Yellow-specific narrative requirement or locked Yellow integration rule. Do not convert this specification into an implementation-status tracker.

### FireRed Retention Specification — review, update only for retention-policy changes

`Pokemon_Golden_Yellow_FireRed_Retention_Specification.md` MUST be reviewed when accepted work affects retained FireRed/Gen III functionality.

Update it only if the accepted work changes, corrects, or materially clarifies a retained feature, modernization requirement, or Yellow/FireRed integration policy. Do not mark ordinary implementation progress here.

### `docs/SUMMARY.md` — update when the documentation set changes

`docs/SUMMARY.md` MUST be updated whenever a project documentation file is created, renamed, moved, or deleted.

Ordinary acceptance does not require a SUMMARY edit when no documentation path changes.

### Acceptance closeout result

At the end of the closeout, the project should be able to answer separately:

- **What is required?** — Master / Hard Rules / Yellow Unique / FireRed Retention.
- **Where are we now?** — Current Project State.
- **What has the user actually accepted?** — Acceptance Log and any linked detailed records.
- **What is actually in code?** — live `prototype/v0.1` repository.

No single document should be forced to answer all four questions.

---

# 19. Scope-Control Rules

## SCOPE-001 — Core Story and Engine Stability Beat Novelty

Before adding a new feature, ask whether it strengthens:

- Yellow fidelity;
- Crystal fidelity;
- cross-region continuity;
- core mechanics;
- long-term endgame quality.

Do not delay core story completion for optional novelty.

## SCOPE-002 — Reuse Before Expansion

Before creating a new subsystem, verify whether an existing FRLG/Emerald/project system can be extended safely.

## SCOPE-003 — Do Not Undermine Regional Identity

Modernization should improve play without erasing the distinction between Yellow-era Kanto, Sevii, Crystal-era Johto, and the later Kanto return.

## SCOPE-004 — Deferred Systems Stay Deferred

Traditional Hoenn Contests and traditional Hoenn Secret Bases are not core requirements unless project direction is explicitly revised.

---

# 20. Current Stage 1A-Specific Protected Principles

These principles are especially important during the opening sequence because Stage 1A is the architecture test for the full project.

## S1A-001 — Solve the Opening Cleanly Before Scaling It

The opening establishes:

- Partner Pikachu identity;
- Oak scripting;
- rival state;
- gift Pokémon architecture;
- follower initialization;
- story flags;
- save persistence;
- special Pokémon restrictions.

Do not accept brittle intro-only hacks that will create systemic problems later.

## S1A-002 — Yellow Opening Fidelity Is a Hard Requirement

The opening must feel recognizably like Pokémon Yellow, not FireRed with Pikachu substituted for a starter.

## S1A-003 — Preserve Yellow-Specific Dialogue Where Distinctive

FRLG may polish presentation, but distinctive Yellow dialogue and character intent should not be overwritten by FRLG wording merely because FRLG text already exists.

## S1A-004 — FRLG Enhancements Are Allowed When Semantically Compatible

Useful FRLG enhancements may remain when they improve presentation without changing Yellow's story meaning, such as:

- GBA movement/facing choreography;
- item/fanfare presentation;
- nickname UI;
- modern battle interface;
- polished Pokédex handoff where semantically equivalent.

For non-exclusive trainer battles, if FRLG preserves the correct major Yellow story role and roster identity, we do not need exact Yellow level/move parity unless there is a gameplay or narrative reason to change it.

## S1A-005 — Yellow Capture Battle Backend Is Protected Unless Directly Implicated

Do not disturb the validated Oak capture battle architecture merely to solve later overworld cutscene issues.

---

# 21. Recommended Decision Order for Future Scene Work

For every future Yellow-to-FRLG scene conversion, follow this order:

1. **Observe Yellow behavior** — gameplay/video/source code.
2. **Identify Yellow invariants** — event order, relative positions, facing, timing, control lock, spawn behavior.
3. **Inspect FRLG equivalent** — map geometry, movement architecture, object lifecycle, existing helpers.
4. **Translate Yellow behavior into FRLG coordinates.**
5. **Prefer script/coordinate changes.**
6. **Reuse existing engine helpers.**
7. **Add narrow engine support only where map scripts cannot safely reproduce the required behavior.**
8. **Protect already-working neighboring behavior.**
9. **Commit separately from build when requested.**
10. **Validate in increasing order:** build → automated checks → manual gameplay → explicit acceptance.

---

# 22. Rule Conflict Resolution

If two rules appear to conflict, use this priority:

1. Explicit latest user direction for the current task.
2. Approved hard rules in this document.
3. Project Master Document canon/architecture.
4. Original source-game authority according to project hierarchy.
5. Existing implementation convenience.

Implementation convenience is never sufficient reason to override project canon or a hard rule.

---

# 23. Governance Summary

The project should continually aim for the smallest, most source-faithful, most maintainable GBA implementation.

The core engineering mindset is:

**Yellow decides the Kanto scene. FireRed supplies proven GBA structure. Translate the original choreography into FRLG coordinates before inventing new systems. Preserve working behavior. Use one clean persistent architecture. Validate each layer honestly. Keep Git history and branch discipline strict.**

