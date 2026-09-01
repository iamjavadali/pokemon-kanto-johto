# Pokémon Golden Yellow — Acceptance Log

**Document type:** Cumulative manual-acceptance ledger  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Log established:** August 26, 2026  
**Last acceptance closeout:** September 1, 2026

---

# 1. Purpose

This document is the canonical cumulative record of Pokémon Golden Yellow gameplay milestones that have been explicitly validated and accepted.

Its role is distinct from `Pokemon_Golden_Yellow_Current_Project_State.md`:

- **Current Project State** says where development broadly stands now.
- **Acceptance Log** records what the user has actually accepted.
- **Dedicated acceptance records** preserve deep implementation/choreography history for complex milestones.

Build success alone is not acceptance.

---

# 2. Acceptance Standard

A milestone is **ACCEPTED** only when the relevant implementation has been tested sufficiently for its stated scope and the user explicitly approves the result.

Evidence may include:

- source commit;
- successful GitHub Actions workflow;
- playable ROM artifact;
- ROM SHA-256;
- targeted manual gameplay test;
- explicit user PASS / FULL PASS / accepted statement.

A later branch HEAD does not invalidate an earlier acceptance merely because development continued afterward.

---

# 3. Regression Rule

Every accepted milestone becomes part of the regression baseline.

Accepted behavior should not intentionally change unless:

1. a regression or hidden defect is discovered;
2. a later required system exposes a genuine integration conflict; or
3. the user explicitly approves a redesign.

When a later fix corrects a defect inside an already accepted milestone, this log records the new accepted regression tip without pretending the original historical acceptance never happened.

---

# 4. Acceptance Index

| Milestone | Status | Detailed record |
|---|---|---|
| Stage 1A Yellow opening / Partner foundation | ACCEPTED | Consolidated historical acceptance |
| Viridian Teachy TV integration | ACCEPTED | This log |
| Pewter / follower-safe early progression | ACCEPTED | This log |
| Pewter Jigglypuff Partner reaction | ACCEPTED | This log |
| Route 3 / Mt. Moon fossil progression | ACCEPTED | `Pokemon_Golden_Yellow_MtMoon_Acceptance_Record.md` |
| Jessie & James encounter #1 | ACCEPTED | `Pokemon_Golden_Yellow_MtMoon_Acceptance_Record.md` |
| Battle follower send-out identity correction | ACCEPTED | This log |
| Cerulean Rival #3 / Fame Checker / Partner choreography | ACCEPTED | This log |
| Route 24 Charmander rescue / Damian adoption | ACCEPTED | `Pokemon_Golden_Yellow_Charmander_Acceptance_Record.md` |
| Bill / Sea Cottage transformed/restored Partner reaction + Yellow portrait slice | ACCEPTED | `Pokemon_Golden_Yellow_Bill_Acceptance_Record.md` |
| Full Yellow Partner portrait catalog + generalized renderer (P1/P2) | ACCEPTED | This log |
| Yellow Partner reaction-program director (P3) | ACCEPTED | This log |
| P7A Pewter Jigglypuff authored Partner integration / sleep lifecycle | ACCEPTED | This log |
| P7B Bill authored A-button precedence integration | ACCEPTED | `Pokemon_Golden_Yellow_Bill_Acceptance_Record.md` + this log |
| P7C Vermilion Pokémon Fan Club authored Partner integration | ACCEPTED | `Pokemon_Golden_Yellow_Fan_Club_Acceptance_Record.md` |
| P7D Pokémon Tower authored fear/unease reaction | ACCEPTED | This log |
| P7E unified Yellow Partner interaction priority / P7 closeout | ACCEPTED | This log |
| Final Pewter/Bill/Fan Club exit-ownership regression closeout | ACCEPTED | Bill/Fan Club records + this log |

---

# 5. Historical Acceptance — Stage 1A Yellow Opening / Partner Foundation

**Status:** ACCEPTED  
**Acceptance type:** Consolidated historical milestone

Accepted scope includes:

- Trainer Watch setup;
- Yellow Pallet departure and Oak interception;
- Oak's dedicated Lv. 5 Pikachu capture;
- Oak/player return to the Lab;
- Rival taking Eevee;
- Oak giving the player the captured Pikachu;
- first Rival battle;
- Rival exit and Partner Pikachu emergence;
- persistent Partner follower activation;
- canonical Partner identity foundation;
- Partner evolution/storage/daycare/trade restrictions;
- Yellow Rival Eevee branching architecture;
- campaign-phase foundation;
- checkpoint/debug foundation.

This acceptance predates the cumulative log and is intentionally not assigned one artificial final SHA.

---

# 6. Acceptance — Viridian Teachy TV Integration

**Status:** ACCEPTED  
**Key source commit:** `ead7f4bfa3b6369206870dd9196576637175236f`  
**Checkpoint persistence correction:** `cd6e54d8c9c4e3d15237d76d9675779550fcb3dc`

Accepted behavior:

- the Old Man does not repeat Oak's already-established catching demonstration;
- the forced interaction resolves correctly;
- Teachy TV is granted;
- progression continues normally;
- reconstructed checkpoints preserve the required state.

---

# 7. Acceptance — Pewter / Follower-Safe Early Progression

**Status:** ACCEPTED

Accepted scope includes:

- pre-Brock Guide redirect behavior;
- Partner-visible scripted escort behavior;
- approved Brock/Pewter Gym baseline;
- post-Brock Running Shoes Aide sequence;
- follower-safe scripted movement foundation.

Regression invariant:

> Accepted scripted scenes must not freeze, duplicate, incorrectly recall, or strand the active Pokémon follower.

---

# 8. Acceptance — Pewter Pokémon Center Jigglypuff Partner Reaction

**Status:** ACCEPTED  
**Final accepted source commit:** `dd60b29aea3ecf3b06fb25da71e6a0002bd0d0a6`  
**Workflow:** `Prototype FireRed Development`  
**Workflow run:** `32089983184`  
**Manual result:** PASS

Accepted result:

- voluntary Jigglypuff interaction triggers the Yellow Partner reaction;
- only canonical Partner Pikachu qualifies;
- sleep/emote realization occurs without applying battle sleep status;
- Partner wakes and normal following resumes.

P7A later supersedes the lifecycle implementation details while preserving this accepted narrative result.

---

# 9. Acceptance — Route 3 / Mt. Moon and Jessie & James Encounter #1

**Status:** ACCEPTED  
**Accepted source commit:** `d70eb1d8905e910076579959ab80c1cf25cae321`  
**Workflow run:** `33009718354`  
**Manual result:** FULL PASS  
**Detailed record:** `Pokemon_Golden_Yellow_MtMoon_Acceptance_Record.md`

Accepted scope includes Miguel/fossil branching, Rocket Meowth theft, Jessie/James encounter #1, Partner integration, blast-off choreography, fossil recovery, and dropped-fossil persistence.

---

# 10. Acceptance — Battle Follower Send-Out Identity Correction

**Status:** ACCEPTED  
**Source commit:** `83bf4dfc394a0ef3a7803bfbdaa464e6912f2922`  
**Workflow run:** `33031832032`  
**Manual result:** PASS

Accepted rule:

- follower-style battle entrance is based on actual resolved follower Pokémon identity;
- ordinary Pikachu cannot substitute for canonical Partner Pikachu;
- another lead Pokémon still uses normal Poké Ball send-out.

---

# 11. Acceptance — Cerulean Rival #3 / Fame Checker / Partner Choreography

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-27  
**Accepted source commit:** `d2ec7b084a0f1951fbcc275bd9ae444a32f57cfd`  
**Workflow run:** `33046024544`  
**Artifact ID:** `9635775412`  
**ROM SHA-256:** `96ebc47ce289d512900edd0d1ba171cab7fb349cf4eaa32b9e9a0b8589f4c16e`  
**Manual result:** ACCEPTANCE PASSED

Accepted scope includes Yellow Rival #3 integration, retained Fame Checker handoff, lane-aware Rival movement, Partner persistence through battle return, safe Partner step-aside timing, and normal following restoration.

---

# 12. Acceptance — Route 24 Charmander Rescue / Damian Adoption

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-27  
**Final manually accepted scene commit:** `1af6360f24b2f638fdd68bfe38bbd869690c4160`  
**Workflow run:** `33134429440`  
**Artifact ID:** `9671564488`  
**Manual result:** PASS  
**Post-acceptance checkpoint correction:** `a4aca426e1d6e3cce57cacf171ea264001f014bd`  
**Checkpoint workflow run:** `33135465513`  
**Checkpoint artifact ID:** `9671932353`  
**Detailed record:** `Pokemon_Golden_Yellow_Charmander_Acceptance_Record.md`

Accepted scope includes abandoned Charmander discovery, Partner concern choreography, Pokémon Center treatment/escape, healed Route 24 return, Damian return, adoption/gift resolution, decline/revisit handling, and the corrected pre-event checkpoint position.

---

# 13. Acceptance — Bill / Sea Cottage Transformed-Restored Partner Reaction

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-29  
**Initial portrait implementation:** `34d7e033dfce7077ee67d593a66a6cef5888b2d7`  
**Historical accepted source commit:** `0cfdbc7ca43ef68ee72dabc75ea8f70834bdc694`  
**Workflow run:** `33256305375`  
**Artifact ID:** `9715960746`  
**ROM SHA-256:** `df93fc0c69cd69e81be03a27c4baef63e5bcd5f0ea78211ee06b0e2d7f96adc0`  
**Manual result:** ACCEPTED PASS  
**Detailed record:** `Pokemon_Golden_Yellow_Bill_Acceptance_Record.md`

Accepted narrative/renderer result:

- transformed Bill → Yellow confused/questioning Partner portrait;
- restored human Bill → Yellow shocked/surprised Partner portrait;
- restored Bill reaches Partner's row before the restored-human reaction;
- original 40×40 Yellow artwork is preserved in the native GBA renderer;
- portraits block scene advancement and clean up deterministically.

Later P7 work extends this accepted Bill milestone with manual A-button precedence, follower lifecycle stabilization, and complete exit guarding without replacing the historical transformed/restored acceptance.

---

# 14. Acceptance — Full Yellow Partner Portrait Catalog and Generalized Renderer (P1/P2)

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-30  
**Generalized renderer commit:** `7d2d0186691c491f1bbacc4ccd114e187401ec7c`  
**Final accepted presentation commit:** `c8d03846f5afbce26e6d55f3dd0180c166b504b5`  
**Workflow run:** `33321231681`  
**Artifact ID:** `9734996200`  
**ROM SHA-256:** `836d93a2c3647f17eb37e864196775bdb9d84dd135aa7551ffe68495a029cbda`  
**Manual result:** PASS

Accepted scope:

- directly selectable Yellow portrait programs `0–28`;
- reconstructed multi-stage visible timelines;
- native 64×64 OBJ carrier around unscaled 40×40 source art;
- corrected centered/tightened portrait frame;
- Pikachu-yellow/gold frame treatment with restoration of normal player window frame;
- deterministic cleanup.

---

# 15. Acceptance — Yellow Partner Reaction-Program Director (P3)

**Status:** ACCEPTED  
**Acceptance date:** 2026-08-30  
**Initial director commit:** `a36b413fd837f9ae982486b48bbcf304d9495f9c`  
**Unified follower-emote commit:** `50772d5cbfee4b0a443d0d5369af21fd46ae3a0b`  
**Include correction:** `653823791b79540acd53a4f33b55840ed55ce2aa`  
**Portrait-synchronized choreography:** `668aaba61d728535fa4431af3a8d08ba62feae5f`  
**Debug facing normalization:** `1d1eaf4bae89f9ccc1d9157c98b457ebfdfc497a`  
**Debug portrait gate:** `019728c9a7ed1f210210901a5d85cbd025685ce6`  
**Native movement executor:** `68224bd0eb71b65f1331a203647b301bdbd97f27`  
**Final accepted source tip:** `598dc0534297c5a7edb099d42a857e2c0eea130a`  
**Workflow run:** `33342904667`  
**Artifact ID:** `9741152434`  
**ROM SHA-256:** `27dee98c48112a7fa15b46c986ea42e711f9373ef049eeea82a2d09e77d6642f`  
**Manual result:** PASS

Accepted scope includes Emotion IDs `0–32`, typed reaction commands, shared portrait/emote/movement infrastructure, tile-safe native `ScriptMovement` body language, deterministic final facing, and canonical Partner-only gating.

---

# 16. Acceptance — P7A Pewter Jigglypuff Authored Partner Integration

**Status:** ACCEPTED  
**Acceptance date:** 2026-09-01  
**Final accepted source tip:** `360470d8f34411b273ac584374c796c4248ef57a`  
**Workflow run:** `33469168689`  
**Manual result:** PASS

Accepted P7A scope:

- common P3 director remains the reaction backend;
- authored sleep lifecycle is repeat-safe;
- Partner object ownership survives blockers, stairs, and escalator cases;
- wake completion restores normal follower authority;
- ordinary Pikachu cannot enter the authored Partner lifecycle.

---

# 17. Acceptance — P7B Bill Authored A-Button Precedence

**Status:** ACCEPTED  
**Acceptance date:** 2026-09-01  
**Accepted source commit:** `29347ce1cfeb769cbf4fd900471742c6e00ff79b`  
**Workflow run:** `33473081407`  
**Artifact ID:** `9787289350`  
**ROM SHA-256:** `2fdb95915613c665ccdd5f380ad771ab63dd191c4197ca21e6d7f75e1c61374e`  
**Manual result:** PASS

Accepted mapping:

- pre-restoration manual Sea Cottage Partner interaction → Emotion `32`;
- post-restoration manual Sea Cottage Partner interaction → Emotion `31`;
- automatic transformed/restored Bill reactions remain owned by the accepted cutscene;
- manual Partner interaction does not mutate Bill story progression.

The later P7 closeout preserves this mapping while stabilizing the live follower-object lifecycle and exit ownership.

---

# 18. Acceptance — P7C Vermilion Pokémon Fan Club Authored Integration

**Status:** ACCEPTED  
**Acceptance date:** 2026-09-01  
**Initial implementation:** `b842434e40b271603a69aa3816e5cc90be0bde55`  
**Choreography alignment:** `cdaffd2e2980a37e0a4b0cf60e6384773c4d77da`  
**Entry timing:** `c55d2cc759784a712387ad4341b1b45b88a4e983`  
**Initial exit guard:** `b237a9d382fb88dc83ebcfe0312ce537c46b85e2`  
**Parked-Partner preservation:** `f0221ae28c19f2a330a83ed86f95aa370395e716`  
**Historical accepted P7C source tip:** `6e62b2d7555f5f9ad20e551c7cd4f399c9b8ea05`  
**Historical workflow run:** `33523794689`  
**Historical artifact ID:** `9806998117`  
**Historical ROM SHA-256:** `2959ae274cd81873be640bf694a5afc562e6ba19e0195cc02ea35f7d5361354a`  
**Historical manual result:** ACCEPTED — user confirmed the scene complete  
**Detailed record:** `Pokemon_Golden_Yellow_Fan_Club_Acceptance_Record.md`

Accepted scene behavior:

- Partner appears beside the player immediately on room entry;
- `SE_PIN` + exclamation pause precedes the walk;
- Partner walks below the ordinary Fan Club Pikachu and faces north;
- automatic Emotion `29` runs through P3;
- Partner remains parked while the room remains usable;
- manual A-button Emotion `30` releases the same follower object back to normal following;
- player cannot leave Partner behind while the parked state is active.

The original P7C exit-guard geometry was later found incomplete for lateral side-warp approaches. The final P7 closeout below replaces that narrow regression detail while preserving the accepted scene.

---

# 19. Acceptance — P7D Pokémon Tower Authored Fear/Unease Reaction

**Status:** ACCEPTED as part of final P7 closeout  
**Implementation commit:** `77921342201b7a813c279f1afdce56087fbc9f2b`  
**Commit message:** `feat: add P7D Pokemon Tower Partner reaction`

Accepted behavior:

- direct interaction with canonical Partner Pikachu on Pokémon Tower 1F–7F selects Yellow Emotion `22` / Tower fear;
- all seven Tower floor maps are explicitly enumerated rather than relying on map-number contiguity;
- the reaction is repeatable;
- it owns no persistent scene state;
- it does not consume a P6 one-shot modifier;
- leaving the Tower naturally returns Partner interaction to the normal lower-priority selection path;
- ordinary Pikachu cannot qualify.

Final manual acceptance evidence is the P7 closeout build in Section 21.

---

# 20. Acceptance — P7E Unified Yellow Partner Interaction Priority

**Status:** ACCEPTED as part of final P7 closeout  
**Implementation commit:** `c9ec8e89c51ed3bd704ed9f02ae035ad5a9f078f`  
**Commit message:** `refactor: unify P7E Partner reaction priority`

P7E establishes one authoritative field-interaction owner before dispatch.

Accepted priority:

1. authored story ownership — Bill, Fan Club, Pewter wake;
2. actual battle/status reaction;
3. authored area reaction — Pokémon Tower;
4. P6 one-shot modifier;
5. P5 friendship/mood fallback.

Accepted invariants:

- lower-priority systems cannot override a higher-priority Yellow reaction;
- P6 state is not consumed unless its selected reaction successfully starts;
- P7A narrative sleep remains separate from actual battle sleep/status;
- Bill's scene-owned visible Partner object remains supported through the common director;
- P7C parked Partner retains direct-interaction ownership until manual release;
- P7D Tower fear remains below actual status but above one-shot/mood selection.

Final manual acceptance evidence is the P7 closeout build in Section 21.

---

# 21. Acceptance — Final P7 Lifecycle and Exit-Ownership Closeout

**Status:** ACCEPTED  
**Acceptance date:** 2026-09-01  
**Final accepted source tip:** `8005312c27b030a451b647cd3ae44f87a1bcad30`  
**Final accepted workflow:** `Prototype FireRed Development`  
**Workflow run:** `33561044203`  
**Artifact ID:** `9821412723`  
**ROM SHA-256:** `2d6f187840c0715112d91e8332854d6b2452beeb119fd1188604cd34d52b2c4b`  
**Manual result:** PASS — user confirmed the build is a pass and authorized documentation closeout before moving to P8

## 21.1 Bill follower-lifecycle stabilization lineage

The P7E integration exposed a Bill scene-owned follower release regression. The accepted stabilization sequence includes:

- `a060cd40bb230c8fdfa5162b44fe0262073f01d0` — `fix: stabilize Bill Partner scene lifecycle`;
- `ca001ff81a965fe1ba51d11a84e6ae19d9e73681` — `fix: place Bill interaction sentinel in EWRAM`;
- `f603bb81f354fcadf05a8c08991f8df15694ed69` — `fix: keep Bill Partner follower continuous`;
- `fa0a8e37b84bc3e491d2d101eee73a607735cda2` — `fix: release Bill Partner follower after reaction`;
- `3c35546b067ed4a4233c078935b9af3d07157fe9` — `fix: detect live Bill Partner follower`.

The accepted result is that the scene-owned/live follower can be interacted with and released back to normal authority without leaving Pikachu frozen or trapping the player in Sea Cottage.

## 21.2 Exit-guard geometry closeout

The first guard-layout correction:

- `f37dab37a2478ce1aae8382b56c03b00d07f4846` — `fix: guard Partner scene side warp exits`

moved the left/right guards onto the actual side warp tiles while retaining exactly three coord events per scene.

Accepted coordinates:

- Pewter Center: center-upper `(7,7)`, side warps `(6,8)` and `(8,8)`;
- Bill's House: center-upper `(7,8)`, side warps `(6,9)` and `(8,9)`;
- Fan Club: center-upper `(5,9)`, side warps `(4,10)` and `(6,10)`.

Manual testing then proved Bill/Fan Club could still bypass a side guard by entering the warp tile laterally because their shared script required `DIR_SOUTH`.

The final correction:

- `8005312c27b030a451b647cd3ae44f87a1bcad30` — `fix: block lateral Partner scene warp exits`

preserves the center-upper south-facing check while routing each Bill/Fan Club side-warp guard through a direction-independent state check.

Pewter required no script change because its guard already blocks from any approach when the authored sleep state is active.

## 21.3 Final accepted exit behavior

- exactly three coord events remain on each affected map;
- center warp definitions remain unchanged;
- Pewter blocks any guarded approach while sleeping Partner state owns the exit;
- Bill center-upper guard remains south-facing only, while left/right warp guards cannot be bypassed laterally when Bill/Partner blocking state owns the exit;
- Fan Club center-upper guard remains south-facing only, while left/right warp guards cannot be bypassed laterally while Partner is parked;
- existing warning text, state checks, and corrective `walk_up` behavior are reused;
- releasing/waking Partner restores normal exit behavior.

## 21.4 P7 closeout boundary

This final PASS closes the authored Partner-reaction P7 phase as the current regression baseline:

- P7A Pewter;
- P7B Bill;
- P7C Fan Club;
- P7D Pokémon Tower;
- P7E unified priority;
- associated scene-owned follower and exit-ownership regression fixes.

The next Partner-system development boundary is P8 modern follower coexistence. That future stage must preserve this complete P7 baseline.

---

# 22. Maintenance Rule

This log is governed by Hard Rule `DOC-006`.

When new gameplay progress is explicitly accepted:

1. update this Acceptance Log;
2. record commit/workflow/ROM evidence when useful;
3. state the tested scope and important regression invariants;
4. update a dedicated acceptance record only when its own milestone requires clarification or regression notes;
5. update Current Project State only when the broad operational boundary materially changes.

The Acceptance Log must not carry a mutable future-work plan beyond identifying the boundary that an acceptance has closed. Current and future development planning belongs in `Pokemon_Golden_Yellow_Current_Project_State.md`.
