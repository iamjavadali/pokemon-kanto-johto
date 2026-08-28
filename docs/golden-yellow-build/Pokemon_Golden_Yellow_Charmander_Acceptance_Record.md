# Pokémon Golden Yellow — Route 24 Charmander Acceptance Record

**Document type:** Dedicated gameplay acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Acceptance date:** August 27, 2026  
**Milestone:** Route 24 Charmander rescue / Damian adoption

---

# 1. Purpose

This record preserves the accepted implementation boundary for Golden Yellow's Route 24 Charmander storyline.

It exists because this milestone spans multiple persistent story states, two maps, Partner Pikachu choreography, lane-aware trigger handling, decline/revisit branches, and a developer checkpoint. Those details are regression-sensitive enough to warrant a dedicated record rather than relying only on the cumulative Acceptance Log.

This record does not replace:

- `Pokemon_Golden_Yellow_Yellow_Unique_Specification.md`, which remains authoritative for the Yellow requirement that Damian's Route 24 Charmander becomes the player's Lv. 10 gift Pokémon;
- `Pokemon_Golden_Yellow_Acceptance_Log.md`, which remains the canonical cumulative acceptance ledger;
- `Pokemon_Golden_Yellow_Current_Project_State.md`, which tracks the current active development boundary.

---

# 2. Acceptance Evidence

**Manual result:** PASS  
**Final manually accepted gameplay commit:** `1af6360f24b2f638fdd68bfe38bbd869690c4160`  
**Commit message:** `fix: polish Charmander interaction facing`  
**Accepted workflow:** `Prototype FireRed Development`  
**Accepted workflow run:** `33134429440`  
**Accepted artifact ID:** `9671564488`

After the gameplay PASS, the user identified one testing-infrastructure issue: the Charmander debug checkpoint landed directly on an event trigger.

**Post-acceptance checkpoint correction:** `a4aca426e1d6e3cce57cacf171ea264001f014bd`  
**Commit message:** `fix: move Charmander checkpoint off trigger`  
**Checkpoint workflow run:** `33135465513`  
**Checkpoint workflow result:** SUCCESS  
**Checkpoint artifact ID:** `9671932353`

The checkpoint-only commit is part of acceptance closeout infrastructure. It does not replace the manual gameplay acceptance evidence attached to `1af6360f…`.

---

# 3. Canon and Adaptation Boundary

Pokémon Yellow requires Damian's Charmander to be obtained on Route 24 as a Lv. 10 gift Pokémon.

Golden Yellow expands the presentation into a rescue arc while preserving that Yellow identity:

1. the player discovers the abandoned/weak Charmander on Route 24;
2. Partner Pikachu participates in the concern scene;
3. the player may agree to take Charmander for treatment;
4. Charmander is treated at the Cerulean Pokémon Center;
5. the healed Charmander later returns to Route 24;
6. Damian returns and explains the abandonment;
7. the storyline resolves into the approved Charmander adoption/gift.

This expanded presentation is an accepted Golden Yellow adaptation, not a replacement of the Yellow gift requirement.

---

# 4. Persistent Story State

The accepted implementation uses `VAR_0x40FC` as the Route 24 Charmander rescue state machine.

Accepted states are:

| Value | Meaning |
|---:|---|
| `0` | Waiting / weak Charmander on Route 24 |
| `1` | Treatment pending / rescue accepted |
| `2` | Healed Charmander returned to Route 24 |
| `3` | Damian present / adoption scene available |
| `4` | Adopted / storyline complete |

Associated object visibility state includes:

- `FLAG_0x0BB` — Route 24 Charmander visibility;
- `FLAG_0x0BC` — Damian visibility;
- `FLAG_0x0BD` — temporary Cerulean Pokémon Center Charmander visibility.

These numeric identifiers are implementation details rather than project-wide semantic names. Future cleanup may replace them with named constants, but the accepted visible behavior and state transitions must remain equivalent.

---

# 5. Accepted Route 24 Discovery Behavior

## 5.1 Idle presentation

Charmander must not stand facing south during ordinary free-roam idle presentation merely because the interaction later requires a south-facing beat.

The accepted scene gives Charmander a neutral/right-facing idle orientation and turns it as part of the interaction itself.

This facing change is intentional choreography: the player sees Charmander react when the scene begins rather than seeing it permanently posed toward the bridge.

## 5.2 Trigger approach

The Route 24 encounter uses staged trigger geometry so the player receives a visible notice/approach sequence before the close interaction.

The accepted close-approach handling is lane-aware. The player and Partner Pikachu are staged relative to the lane from which the encounter is entered rather than assuming one universal starting coordinate.

The choreography must not depend on an unverified assumption that the automatic follower-copy system will always place Partner Pikachu at one exact coordinate after a long scripted player route.

---

# 6. Accepted Partner Pikachu Choreography

Partner Pikachu is a visible participant in the Charmander storyline.

Accepted requirements include:

- Partner Pikachu remains represented by the canonical Partner/follower system;
- ordinary Pikachu must not substitute for the canonical Partner in Yellow-specific logic;
- scripted Route 24 staging must avoid recalling Partner Pikachu solely to clear scene geometry;
- Partner Pikachu must be placed deliberately during the weak-Charmander concern beat;
- lane-specific staging must use known walkable tiles;
- during the healed-return/Damian sequence, Partner Pikachu must remain behind the player and outside Damian's approach path;
- normal following must resume after controlled choreography completes.

The final accepted implementation uses scene-local movement and follower-safe handling rather than redesigning the generic follower engine for this one event.

---

# 7. Accepted Weak-Charmander Decision Flow

When the player reaches the close interaction with the weak Charmander:

1. the player and Partner are staged for the conversation;
2. Charmander acknowledges the player;
3. Charmander and Partner Pikachu visibly interact during the concern beat;
4. the player is asked whether to help/take Charmander for treatment;
5. accepting advances the persistent rescue state;
6. declining leaves the storyline available rather than consuming the event.

Decline paths must restore coherent object facing and release player/follower control cleanly.

A decline must not:

- hide Charmander permanently;
- grant the gift;
- advance directly to Damian;
- strand Partner Pikachu in scene staging;
- corrupt the rescue state.

---

# 8. Accepted Cerulean Pokémon Center Treatment Sequence

The rescue continues inside the Cerulean Pokémon Center using a temporary Charmander object tied to the Route 24 rescue state.

Accepted visible behavior includes:

- player and Partner Pikachu are staged predictably for the treatment scene;
- the temporary Charmander appears in the treatment context;
- Nurse Joy reacts when Charmander moves unexpectedly;
- Charmander's movement path avoids incorrectly crossing through or displacing Partner Pikachu;
- the temporary Center Charmander is removed when the treatment sequence completes;
- the persistent rescue state advances so Route 24 can reconstruct the healed-return phase;
- player and Partner facing/following are restored coherently after the sequence.

The Pokémon Center scene is not an independent one-off cutscene. It is one stage of the same persistent Route 24 story state.

---

# 9. Accepted Healed Return and Damian Sequence

When the rescue state reaches the healed-return phase:

- Charmander is again present on Route 24;
- the player can trigger the return interaction using the accepted Route 24 approach geometry;
- Partner Pikachu is staged behind the player for the Damian sequence;
- Damian appears only at the appropriate later state;
- Damian approaches without colliding with Partner Pikachu;
- the scene communicates Damian's abandonment context;
- the adoption/gift resolution remains available until accepted;
- accepting the final outcome grants Charmander at Lv. 10 and records one-time completion.

Once the story reaches the adopted/completed state, the rescue/adoption sequence must not replay as though Charmander were still abandoned.

---

# 10. Checkpoint Acceptance Closeout

The permanent debug selection remains `GY_DEBUG_CP_CHARMANDER_GIFT`.

Its role is to reconstruct the pre-event Route 24 state for direct testing. It is not production story logic.

The checkpoint originally warped the player to Route 24 `(7,9)`, which landed directly on the newly accepted event trigger boundary.

Post-acceptance correction `a4aca426…` moved the warp to:

**Route 24 `(7,10)`**

This gives the tester control south of the event before walking into the accepted trigger geometry.

Regression invariant:

> The Charmander checkpoint must not be placed directly on a coordinate trigger that starts the scene immediately upon checkpoint warp-in.

---

# 11. Rejected / Superseded Approaches

The following approaches are not part of the accepted baseline and should not be restored casually:

- Charmander permanently idling south-facing before interaction;
- long encounter choreography that assumes automatic follower-copy plus a fixed delay always yields one exact Partner coordinate;
- staging Partner Pikachu from a coordinate that was never actually guaranteed by the follower engine;
- allowing Partner Pikachu to remain in Damian's movement lane during the return scene;
- moving the accepted Route 24 trigger boundary merely to compensate for a bad developer-checkpoint spawn;
- placing the checkpoint itself directly on the event trigger.

If future work exposes a real engine conflict, preserve the accepted visible behavior while redesigning the implementation only as narrowly as necessary.

---

# 12. Regression Baseline

Future Cerulean, Route 25, Bill, Bulbasaur, Misty, or later follower work must preserve the following Charmander invariants unless the user explicitly approves a redesign:

1. Route 24 remains the canonical Yellow Charmander location.
2. Charmander is obtained as the accepted Lv. 10 one-time gift/adoption outcome.
3. The weak rescue, Pokémon Center treatment, healed return, Damian return, and final adoption remain one coherent persistent storyline.
4. Charmander does not default to the rejected south-facing idle pose.
5. Partner Pikachu remains visibly integrated without ordinary Pikachu being treated as the Partner.
6. Route 24 scripted staging must remain safe for the active follower.
7. Damian does not appear early and does not collide with Partner Pikachu during his approach.
8. Decline/revisit paths preserve the event rather than consuming or corrupting it.
9. The developer checkpoint remains south of the event trigger and reconstructs the correct pre-event state.

---

# 13. Closeout

**Milestone status:** ACCEPTED

The accepted Yellow story boundary now extends through the Route 24 Charmander rescue / Damian adoption sequence.

The next active Cerulean-era development boundary is Route 25 progression into Bill / Sea Cottage and the required Partner Pikachu reaction to Bill's transformation/restoration sequence.
