# Pokémon Golden Yellow — Mt. Moon Acceptance Record

**Document type:** Detailed milestone acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Branch:** `prototype/v0.1`  
**Accepted source commit:** `d70eb1d8905e910076579959ab80c1cf25cae321`  
**Accepted commit message:** `Add faster walking and jumping animations for characters`  
**Accepted workflow:** `Prototype FireRed Development`  
**Accepted workflow run:** `33009718354`  
**Accepted artifact ID:** `9622158159`  
**Accepted ROM SHA-256:** `764a21f40881bd4e9b47ddcd6442126ee87b40ebf95e2af58fa285f2c9c101fd`  
**Acceptance date:** 2026-08-26  
**Manual gameplay result:** FULL PASS

---

# 1. Purpose

This record preserves the detailed accepted implementation state for the Route 3 / Mt. Moon fossil progression and the first Yellow-specific Jessie, James, and Meowth encounter.

It is a **deep milestone record**, not the project's cumulative acceptance ledger.

Project-wide acceptance history is maintained in:

`Pokemon_Golden_Yellow_Acceptance_Log.md`

This document should only be updated when the accepted Mt. Moon milestone itself needs clarification, regression notes, or an explicitly approved redesign.

---

# 2. Accepted Scope

The accepted scene covers:

- Miguel fossil battle and fossil-selection flow;
- both fossil-choice branches;
- Meowth theft of the unchosen fossil;
- branch-specific Meowth staging for Dome-first and Helix-first paths;
- Miguel chase and help prompt;
- Jessie, James, and Rocket Meowth overworld staging;
- Jessie and James overworld sprites using the stable 32x32 object graphics path;
- Team Rocket music cues;
- Rocket dialogue tying the fossil theft to Giovanni/Boss pressure;
- Rocket dialogue noticing canonical Partner Pikachu because it remains outside its Poké Ball;
- Rocket Meowth recognizing that Partner Pikachu chooses who to follow;
- Jessie/James battle flow;
- post-battle stolen fossil drop;
- improved script-level Team Rocket blast-off animation;
- Miguel recovery entrance and permission dialogue;
- manual pickup of the dropped fossil;
- dropped-fossil persistence if the player leaves before pickup;
- follower-safe Partner Pikachu behavior through the scripted sequence.

---

# 3. Accepted Gameplay Behavior

The user manually tested the accepted build and reported that the scene is a full pass.

The final accepted flow is:

1. Player defeats Miguel and chooses one fossil.
2. Miguel moves to take the other fossil.
3. Rocket Meowth enters and steals the unchosen fossil.
4. Miguel reacts, chases Meowth, and calls for help.
5. Jessie, James, and Rocket Meowth are staged for the first Yellow Rocket encounter.
6. Team Rocket dialogue establishes:
   - the fossil was being stolen for the Boss;
   - they fear returning empty-handed;
   - Partner Pikachu is unusual because it stays outside the Poké Ball;
   - Rocket Meowth understands that Partner Pikachu is choosing who to follow.
7. The Jessie/James battle runs.
8. Team Rocket drops the stolen fossil after losing.
9. Team Rocket performs the accepted blast-off animation and exits.
10. Miguel returns, recognizes the recovered fossil, and permits the player to keep it.
11. The player manually picks up the dropped fossil.

---

# 4. Protected Decisions From This Pass

The following decisions are part of the accepted Mt. Moon regression baseline unless a regression appears or the user explicitly requests a redesign:

- Keep the stable 32x32 overworld object graphics path for Jessie and James.
- Do not revert Jessie/James to placeholder doll sprites.
- Do not reintroduce the failed 16x32 OAM/subsprite pipeline for Jessie and James.
- Do not reintroduce unsafe `spin_up`, `fly_up`, Teleport, or full Fly field-effect behavior for the Mt. Moon blast-off.
- The accepted blast-off remains script-level choreography, not a custom field effect.
- Dropped fossil pickup remains manual.
- The repeated sign-style “TEAM ROCKET dropped the stolen fossil” message remains removed from pickup.
- Partner Pikachu must remain out and follower-safe through the cutscene.
- Canonical Partner Pikachu logic must continue to resolve `SPECIES_PIKACHU_STARTER`, not ordinary `SPECIES_PIKACHU`.

---

# 5. Related Implementation Surfaces

Primary map/script surface:

- `data/maps/MtMoon_B2F_Frlg/scripts.inc`

Related graphics/object-event surfaces from the accepted sprite work:

- `graphics/object_events/pics/people/jessie.png`
- `graphics/object_events/pics/people/james.png`
- `src/golden_yellow_jessie_james_overworld.c`

Do not modify the graphics/object-event surfaces for future Mt. Moon choreography polish unless the change is explicitly sprite-related.

---

# 6. Project-State Relationship

This record establishes that the following broad project areas are complete for the accepted Mt. Moon milestone:

- Route 3 / Mt. Moon fossil progression;
- Jessie & James encounter #1.

`Pokemon_Golden_Yellow_Current_Project_State.md` should only summarize those areas as implemented/accepted and should not duplicate this record's choreography, hashes, or detailed regression notes.

`Pokemon_Golden_Yellow_Acceptance_Log.md` carries the cumulative project-wide acceptance entry and links back to this detailed record.

---

# 7. Remaining Jessie & James Work

This accepted implementation covers only Jessie/James encounter #1.

The following Yellow Jessie/James encounters remain required later work:

1. Rocket Hideout B4F;
2. Pokémon Tower 7F;
3. Silph Co. 11F.

Checkpoint existence does not imply implementation or acceptance.

---

# 8. Validation Rule

Build success alone did not make this scene accepted.

This scene is accepted because:

- the build passed;
- the ROM artifact was pulled and tested;
- both fossil branches and the final blast-off path were manually validated;
- the user explicitly reported: `This is now a full pass.`

The accepted source commit remains the historical milestone reference even as later development commits advance the branch.
