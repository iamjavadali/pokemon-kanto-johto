# Pokémon Golden Yellow — Vermilion Pokémon Fan Club Acceptance Record

**Document type:** Dedicated manual-acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Acceptance date:** September 1, 2026  
**Milestone:** P7C Vermilion Pokémon Fan Club authored Partner integration  
**Status:** ACCEPTED

---

# 1. Acceptance Summary

P7C is manually accepted as the completed Golden Yellow implementation of Pokémon Yellow's Vermilion Pokémon Fan Club Partner Pikachu sequence.

The accepted result preserves the existing FireRed Fan Club room, Chairman/Bike Voucher progression, and ordinary Fan Club Pikachu while layering the Yellow-specific Partner choreography onto the canonical Partner Pikachu (`SPECIES_PIKACHU_STARTER`).

The scene uses the existing Golden Yellow follower object, P3 reaction director, portrait system, and P7A follower-ownership safeguards. It does not create a second Partner engine.

---

# 2. Final Acceptance Evidence

**Initial P7C implementation:** `b842434e40b271603a69aa3816e5cc90be0bde55`  
**Choreography alignment fix:** `cdaffd2e2980a37e0a4b0cf60e6384773c4d77da`  
**Room-entry timing fix:** `c55d2cc759784a712387ad4341b1b45b88a4e983`  
**Initial exit-guard implementation:** `b237a9d382fb88dc83ebcfe0312ce537c46b85e2`  
**Parked-Partner exit preservation fix:** `f0221ae28c19f2a330a83ed86f95aa370395e716`  
**Final accepted source tip / exclamation timing fix:** `6e62b2d7555f5f9ad20e551c7cd4f399c9b8ea05`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33523794689`  
**Accepted artifact ID:** `9806998117`  
**Accepted ROM SHA-256:** `2959ae274cd81873be640bf694a5afc562e6ba19e0195cc02ea35f7d5361354a`  
**Manual result:** ACCEPTED — user explicitly confirmed, “this is now complete.”

---

# 3. Source Behavior Preserved

The Yellow source behavior translated into Golden Yellow is:

1. canonical Partner Pikachu enters the Fan Club with the player;
2. Partner visibly appears beside the player immediately on room entry;
3. Partner notices the existing Fan Club Pikachu;
4. the `!` attention beat appears with the standard attention sound and a readable pause;
5. Partner walks toward the Fan Club Pikachu;
6. Partner stops directly below the Fan Club Pikachu and faces north;
7. the automatic Yellow Fan Club affection reaction executes through the common P3 reaction director;
8. Partner remains parked with the other Pikachu while the player can move around the room and use the normal Chairman/Bike Voucher flow;
9. the player cannot leave the room while Partner is still parked;
10. pressing A on the parked Partner executes the Yellow manual Fan Club reaction;
11. the authored scene releases Partner back to normal `FOLLOW_PLAYER` behavior;
12. normal room exit becomes available after Partner rejoins.

The automatic Fan Club reaction uses the existing Golden Yellow mapping for Yellow Emotion `29` (`GY_PARTNER_REACTION_FAN_CLUB_AFFECTION`).

The manual parked-Partner reaction uses the existing Golden Yellow mapping for Yellow Emotion `30` (`GY_PARTNER_REACTION_FAN_CLUB_MAX_AFFECTION`).

---

# 4. Accepted Entry Choreography

The accepted scene does not wait for the player to walk several tiles north before starting.

The final implementation follows the proven Bill-house map-entry lifecycle:

- `MAP_SCRIPT_ON_TRANSITION` arms the Fan Club entry reaction;
- `MAP_SCRIPT_ON_FRAME_TABLE` consumes the pending reaction on the first controllable map frame;
- the player is locked before normal room movement begins;
- the follower object is given a brief map-entry settle period;
- P7C transfers ownership of the existing follower object using the accepted P7A handoff;
- Partner is staged beside the player;
- the authored attention and approach sequence begins immediately.

The removed `y=8` arrival coordinate triggers must not be restored as the primary entry mechanism, because they caused the scene to begin only after the player had already walked into the room.

---

# 5. Accepted Exclamation Beat

The final accepted attention beat is deliberately separated from the walk.

Partner Pikachu:

- receives the standard `SE_PIN` attention sound;
- displays the exclamation emote;
- holds the beat for three 16-frame delays (48 frames total);
- then begins the computed walk toward the Fan Club Pikachu.

This timing is part of the accepted choreography and should not be collapsed back into an immediate `!`-then-walk transition without explicit redesign approval.

---

# 6. Accepted Approach and Parked Position

The existing ordinary Fan Club Pikachu remains the room's normal `OBJ_EVENT_GFX_PIKACHU_FRLG` object at `(7,6)`.

The canonical Partner's authored destination is the tile directly south of that Pikachu:

- target X: `clubPikachu->currentCoords.x`;
- target Y: `clubPikachu->currentCoords.y + 1`;
- accepted room position: `(7,7)`;
- final facing: north.

The earlier target left of the ordinary Pikachu is rejected because it visually moved Partner into the table area rather than below the admired Pikachu.

After the automatic Emotion `29` reaction completes, P7C re-parks the existing follower object at the accepted position and keeps the saved P7C state as `PARKED` until the manual Emotion `30` release interaction completes.

---

# 7. Accepted Exit Prevention

The player must not be allowed to abandon the scene-owned Partner Pikachu in the Fan Club.

The room's existing `y=9` interior doorway-row coordinate events are retained as the exit interception point.

Because the same doorway row is also involved in room entry, P7C must distinguish an actual exit attempt from merely standing on the row after entering.

The accepted guard therefore blocks only when:

- P7C Partner state is `PARKED`; and
- the player is facing `DIR_SOUTH` toward the exit.

When blocked:

1. `FLAG_SAFE_FOLLOWER_MOVEMENT` is set;
2. the parked Partner is explicitly restored/normalized as the visible scene-owned follower object;
3. Partner Pikachu cries;
4. the player receives the warning that Pikachu is still busy with the other Pikachu and should not be left behind;
5. the player is walked one tile north back into the room;
6. the parked Partner is explicitly restored again;
7. `FLAG_SAFE_FOLLOWER_MOVEMENT` is cleared;
8. Partner remains visible, parked, and interactable.

The earlier implementation that checked only `PARKED` state is rejected because it falsely treated the post-entry doorway position as an exit attempt.

The earlier forced-backstep implementation without `FLAG_SAFE_FOLLOWER_MOVEMENT` is also rejected because scripted player movement could recall/hide the scene-owned follower into its Poké Ball while leaving the logical P7C state parked and interactable.

---

# 8. Accepted Manual Release / Rejoin

While Partner is parked, direct A-button interaction has authored precedence over normal Partner talk selection.

The parked interaction dispatches Yellow Emotion `30` through the shared P3 field-talk reaction path.

After the reaction finishes:

- `GoldenYellow_CompleteFanClubPartnerOnFollower()` normalizes the complete follower movement state;
- the same object returns to `MOVEMENT_TYPE_FOLLOW_PLAYER`;
- Partner visibility is restored explicitly;
- P7C saved scene state is cleared;
- normal Partner following resumes;
- the room exits are no longer blocked.

This is the single normal P7C completion path.

---

# 9. Preserved FireRed / Existing Golden Yellow Behavior

P7C must continue to preserve:

- the existing Fan Club room layout;
- the ordinary Fan Club Pikachu and its normal cry/dialogue interaction;
- the Chairman's existing Bike Voucher flow;
- other Fan Club NPC dialogue and behavior;
- canonical Partner identity rules;
- P1/P2 portrait rendering;
- P3 reaction programs;
- P4/P5/P6 Partner interaction infrastructure;
- P7A follower movement ownership and cleanup invariants;
- P7B authored precedence behavior at Bill's Sea Cottage.

P7C does not create a second follower, second portrait system, or second reaction engine.

---

# 10. Regression Invariants

Future work must preserve all of the following unless the user explicitly approves a redesign:

- only `SPECIES_PIKACHU_STARTER` may qualify as canonical Partner Pikachu;
- ordinary `SPECIES_PIKACHU` must never inherit Partner-only Fan Club reactions;
- the scene must begin immediately on Fan Club room entry rather than after several player steps;
- Partner must visibly appear beside the player before the attention beat;
- the exclamation beat must include `SE_PIN` and a readable pause before movement;
- Partner must stop directly below the ordinary Fan Club Pikachu and face north;
- the automatic Fan Club reaction must remain Yellow Emotion `29` through the shared P3 director;
- Partner must remain visible and parked after the automatic reaction;
- the player must remain free to use the normal Fan Club room and Chairman/Bike Voucher interaction while Partner is parked;
- attempting to leave while parked must be blocked only for a genuine south-facing exit attempt;
- the exit-block player backstep must use `FLAG_SAFE_FOLLOWER_MOVEMENT` and restore the parked Partner so it cannot be recalled/invisibly stranded;
- direct A-button interaction with the parked Partner must dispatch Yellow Emotion `30`;
- Emotion `30` completion must restore normal `FOLLOW_PLAYER` authority and clear the P7C parked state;
- Chairman/Bike Voucher and ordinary Fan Club Pikachu behavior must not be rewritten by later Partner work;
- P7A and P7B remain protected regression baselines.

---

# 11. Acceptance Boundary

P7C is now part of the manually accepted Golden Yellow regression baseline.

This acceptance does not automatically accept later Vermilion progression, S.S. Anne, Lt. Surge, Squirtle acquisition, or the later Pokémon Tower Partner fear/unease authored reaction. Those remain separate development/acceptance boundaries and must be audited and validated independently.
