# Pokémon Golden Yellow — Vermilion Pokémon Fan Club Acceptance Record

**Document type:** Dedicated manual-acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Original P7C acceptance date:** September 1, 2026  
**Final exit-guard regression closeout:** September 1, 2026  
**Milestone:** P7C Vermilion Pokémon Fan Club authored Partner integration  
**Status:** ACCEPTED

---

# 1. Acceptance Summary

P7C is the accepted Golden Yellow implementation of Pokémon Yellow's Vermilion Pokémon Fan Club Partner Pikachu sequence.

The implementation preserves the existing FireRed Fan Club room, Chairman/Bike Voucher progression, and ordinary Fan Club Pikachu while layering Yellow-specific choreography onto canonical Partner Pikachu (`SPECIES_PIKACHU_STARTER`).

The scene uses the existing Golden Yellow follower object, P3 reaction director, portrait system, and P7 follower-ownership safeguards. It does not create a second Partner engine.

The original P7C scene was manually accepted before a later regression test exposed incomplete exit-guard coverage for lateral entry onto the side warp tiles. The final P7 closeout corrected that narrow guard defect without changing the accepted Fan Club choreography or Emotion `29` / Emotion `30` behavior.

---

# 2. Original P7C Acceptance Evidence

**Initial P7C implementation:** `b842434e40b271603a69aa3816e5cc90be0bde55`  
**Choreography alignment fix:** `cdaffd2e2980a37e0a4b0cf60e6384773c4d77da`  
**Room-entry timing fix:** `c55d2cc759784a712387ad4341b1b45b88a4e983`  
**Initial exit-guard implementation:** `b237a9d382fb88dc83ebcfe0312ce537c46b85e2`  
**Parked-Partner exit preservation fix:** `f0221ae28c19f2a330a83ed86f95aa370395e716`  
**Historical accepted source tip / exclamation timing fix:** `6e62b2d7555f5f9ad20e551c7cd4f399c9b8ea05`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33523794689`  
**Artifact ID:** `9806998117`  
**ROM SHA-256:** `2959ae274cd81873be640bf694a5afc562e6ba19e0195cc02ea35f7d5361354a`  
**Manual result:** ACCEPTED — user explicitly confirmed, “this is now complete.”

---

# 3. Source Behavior Preserved

The accepted Yellow behavior translated into Golden Yellow is:

1. canonical Partner Pikachu enters the Fan Club with the player;
2. Partner visibly appears beside the player immediately on room entry;
3. Partner notices the existing Fan Club Pikachu;
4. the `!` attention beat appears with the standard attention sound and a readable pause;
5. Partner walks toward the Fan Club Pikachu;
6. Partner stops directly below the ordinary Fan Club Pikachu and faces north;
7. the automatic Yellow Fan Club affection reaction executes through the common P3 director;
8. Partner remains parked with the other Pikachu while the player can move around the room and use the normal Chairman/Bike Voucher flow;
9. the player cannot leave while Partner remains parked;
10. pressing A on the parked Partner executes the Yellow manual Fan Club reaction;
11. the same follower object returns to normal `FOLLOW_PLAYER` authority;
12. normal room exit becomes available after Partner rejoins.

Automatic Fan Club reaction:

- Yellow Emotion `29` — `GY_PARTNER_REACTION_FAN_CLUB_AFFECTION`.

Manual parked-Partner release reaction:

- Yellow Emotion `30` — `GY_PARTNER_REACTION_FAN_CLUB_MAX_AFFECTION`.

---

# 4. Accepted Entry Choreography

The scene starts on room entry rather than waiting for the player to walk several tiles north.

Accepted lifecycle:

- `MAP_SCRIPT_ON_TRANSITION` arms the entry reaction;
- `MAP_SCRIPT_ON_FRAME_TABLE` consumes the pending reaction on the first controllable map frame;
- the player is locked before normal room movement begins;
- the follower receives a brief entry settle period;
- P7C transfers authored ownership of the existing follower object;
- Partner is staged beside the player;
- the attention/approach sequence begins immediately.

The removed `y=8` arrival coordinate triggers must not be restored as the primary entry mechanism.

---

# 5. Accepted Exclamation Beat

Partner Pikachu:

- receives `SE_PIN`;
- displays the exclamation emote;
- holds the attention beat for three 16-frame delays (48 frames total);
- then begins the authored approach.

This timing is part of the accepted choreography.

---

# 6. Accepted Approach and Parked Position

The ordinary Fan Club Pikachu remains at `(7,6)`.

The canonical Partner destination is directly south of it:

- target X: `clubPikachu->currentCoords.x`;
- target Y: `clubPikachu->currentCoords.y + 1`;
- accepted room position: `(7,7)`;
- final facing: north.

After automatic Emotion `29`, the same follower object remains visibly parked at the accepted position until manual Emotion `30` completes.

---

# 7. Final Accepted Exit Prevention

The Fan Club exit is three tiles wide:

- left warp `(4,10)`;
- center warp `(5,10)`;
- right warp `(6,10)`.

The accepted scene retains exactly **three coordinate guard events**, but they do not all use the same directional rule.

Final accepted guard placement:

- center-upper guard: `(5,9)`;
- left side-warp guard: `(4,10)`;
- right side-warp guard: `(6,10)`.

## 7.1 Center-upper guard

The center-upper `(5,9)` guard keeps the existing P7C behavior:

- Partner state must be `PARKED`;
- player must be facing `DIR_SOUTH` toward the exit;
- then the existing block routine owns the attempt.

The facing check is necessary on this shared interior doorway tile so ordinary movement across the room is not falsely treated as an exit attempt.

## 7.2 Side-warp guards

The left `(4,10)` and right `(6,10)` guards sit directly on the side warp tiles.

When Partner state is `PARKED`, these side guards are **direction-independent**.

This is required because the player can approach either side warp laterally while facing east or west. A south-only test on those tiles permits the guard script to finish and allows the warp to fire.

The accepted side guards therefore check the P7C parked state but do not require `DIR_SOUTH` before entering the existing block routine.

## 7.3 Existing block behavior remains unchanged

When an exit is blocked:

1. `FLAG_SAFE_FOLLOWER_MOVEMENT` is set;
2. the parked Partner is explicitly restored/normalized as the visible scene-owned follower object;
3. Partner Pikachu cries;
4. the player receives the existing warning that Pikachu is still busy with the other Pikachu and should not be left behind;
5. the player is moved one tile north with the existing corrective `walk_up` movement;
6. the parked Partner is restored again;
7. `FLAG_SAFE_FOLLOWER_MOVEMENT` is cleared;
8. Partner remains visible, parked, and interactable.

No additional guard events, new warning behavior, or C-level exit subsystem is required.

---

# 8. Exit-Guard Regression History

The original P7C acceptance used a south-facing exit test on the interior doorway row. Later manual regression testing showed that the three-wide warp could be bypassed from the left/right lanes.

First correction:

- `f37dab37a2478ce1aae8382b56c03b00d07f4846` — `fix: guard Partner scene side warp exits`.

That moved the outer guards from `(4,9)` / `(6,9)` onto the actual side warp tiles `(4,10)` / `(6,10)` while retaining the center-upper `(5,9)` guard.

Manual testing then proved a second issue: lateral east/west entry onto a side warp still bypassed the guard because all three events were routed through the same `DIR_SOUTH` condition.

Final correction:

- `8005312c27b030a451b647cd3ae44f87a1bcad30` — `fix: block lateral Partner scene warp exits`.

That commit routes the two side-warp guards through a direction-independent parked-state check while leaving the center-upper guard's south-facing semantics intact.

---

# 9. Final Exit-Guard Acceptance Evidence

**Final accepted source tip:** `8005312c27b030a451b647cd3ae44f87a1bcad30`  
**Accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33561044203`  
**Artifact ID:** `9821412723`  
**ROM SHA-256:** `2d6f187840c0715112d91e8332854d6b2452beeb119fd1188604cd34d52b2c4b`  
**Manual result:** PASS — user confirmed the final build passed

The final PASS closes the lateral side-warp bypass while preserving the already accepted Fan Club entry, parked state, manual release, and Chairman/Bike Voucher behavior.

---

# 10. Accepted Manual Release / Rejoin

While Partner is parked, direct A-button interaction has authored precedence over normal Partner talk selection.

The parked interaction dispatches Yellow Emotion `30` through the shared P3 field-talk reaction path.

After the reaction completes:

- the follower movement state is normalized;
- the same object returns to `MOVEMENT_TYPE_FOLLOW_PLAYER`;
- Partner visibility is restored;
- the P7C saved parked state is cleared;
- normal following resumes;
- all three room exits become usable normally.

This remains the normal P7C completion path.

---

# 11. Preserved FireRed / Golden Yellow Behavior

P7C preserves:

- the existing Fan Club room layout;
- the ordinary Fan Club Pikachu and its normal interaction;
- the Chairman's Bike Voucher flow;
- other Fan Club NPC behavior;
- canonical Partner identity;
- P1/P2 portrait rendering;
- P3 reaction programs;
- P4/P5/P6 Partner interaction infrastructure;
- P7A follower ownership/cleanup invariants;
- P7B Bill authored precedence;
- P7E single-owner reaction priority.

P7C does not create a second follower, portrait system, or reaction engine.

---

# 12. Regression Invariants

Future work must preserve:

- only `SPECIES_PIKACHU_STARTER` qualifies as canonical Partner Pikachu;
- ordinary `SPECIES_PIKACHU` never inherits Partner-only Fan Club behavior;
- the scene starts immediately on Fan Club room entry;
- Partner appears beside the player before the attention beat;
- `SE_PIN` and the readable exclamation pause remain before movement;
- Partner stops at `(7,7)` below the ordinary Fan Club Pikachu and faces north;
- automatic Fan Club reaction remains Emotion `29` through the common P3 director;
- Partner remains visible and parked afterward;
- the player remains free to use the Chairman/Bike Voucher flow while Partner is parked;
- exactly three exit coord events remain unless redesign is explicitly approved;
- center-upper guard `(5,9)` blocks only a genuine south-facing exit attempt while parked;
- side-warp guards `(4,10)` and `(6,10)` block from any approach direction while parked;
- neither side warp may be bypassed by lateral east/west entry;
- the block routine keeps `FLAG_SAFE_FOLLOWER_MOVEMENT`, explicit Partner restoration, warning, and corrective `walk_up` behavior;
- manual A-button interaction with parked Partner remains Emotion `30`;
- Emotion `30` completion restores normal `FOLLOW_PLAYER` authority and clears the parked state;
- ordinary room behavior must not be rewritten by P8 or later Partner work.

---

# 13. Acceptance Boundary

P7C, including its final side-warp exit-guard correction, is part of the accepted P7 regression baseline.

This does not automatically accept S.S. Anne, Lt. Surge, Squirtle acquisition, or other remaining Vermilion progression.

P8 modern follower coexistence must preserve this complete Fan Club ownership and exit behavior while layering compatible contextual follower behavior below the accepted Yellow priority system.
