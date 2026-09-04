# Pokémon Golden Yellow — Partner Blueprint Status Addendum

**Status date:** September 4, 2026  
**Current boundary:** **P9B audio baseline accepted → reaction corrections and P9 closeout active**

This file began as the source-audit and implementation blueprint for the Yellow Partner portrait/reaction subsystem. The audit and original design language below are intentionally preserved as historical technical rationale.

Where the original blueprint speaks in future tense, the current implementation status is now:

- **P1/P2 — ACCEPTED:** complete portrait catalog and generalized renderer;
- **P3 — ACCEPTED:** common reaction-program director;
- **P4 — IMPLEMENTED:** normal canonical Partner A-button routing;
- **P5 — IMPLEMENTED:** persistent friendship/mood lifecycle; Golden Yellow initializes uninitialized Partner mood to neutral `128` and untouched canonical Partner friendship to `90`;
- **P6 — IMPLEMENTED:** supported one-shot modifier architecture;
- **P7A–P7E — ACCEPTED:** Pewter, Bill, Fan Club, Pokémon Tower, and unified Story > Status > Area > One-shot > Mood ownership priority;
- **final P7 regression closeout — ACCEPTED:** Bill live-follower release and complete Pewter/Bill/Fan Club exit guarding, including lateral side-warp protection;
- **P8 — ACCEPTED:** modern contextual follower behavior is layered underneath the accepted Yellow priority system; ordinary followers remain unchanged, Yellow-owned Partner reactions remain exclusive, and direct local context outranks nearby/ambient context;
- **P9A — LOCKED:** Option B reachability policy preserves IDs `01–42`, authorizes 27 Required clips for initial import, defers one clip, excludes 14 Not Applicable clips, and defines Yellow `$FF` as true `NO_PCM`;
- **P9B — ACCEPTED FOR CURRENT AUDIO SCOPE:** 27 Required PCM clips are imported and wired; true `NO_PCM`, separate outer/portrait timing, filtered assets, tiered volume, and non-ducking BGM overlay are established. The active boundary is correction of newly reported reaction defects before full P9 closeout.

The original source-audit caveat about the exact **original Pokémon Yellow** mood initialization writer remains unresolved as a source-history question. It no longer blocks Golden Yellow implementation because P5 has an explicit project implementation choice as described above.

For exact acceptance commits, workflow runs, ROM hashes, and final regression evidence, use `Pokemon_Golden_Yellow_Acceptance_Log.md` and the dedicated acceptance records. For the authoritative P9 audio classifications, timing distinctions, and ROM budget, use `Pokemon_Golden_Yellow_P9A_Audio_Reachability_Audit.md`. This addendum supersedes only stale implementation-status statements below; it does not replace the source analysis or architectural rules.

---

# 1. Yellow subsystem source map

The authoritative Yellow implementation is spread across several coordinated files.

| Layer                         | Yellow source                              | Responsibility                                                                |
| ----------------------------- | ------------------------------------------ | ----------------------------------------------------------------------------- |
| Interaction/reaction director | `engine/pikachu/pikachu_emotions.asm`      | A-button talk, reaction precedence, status/map overrides, emotion interpreter |
| Emotion programs              | `data/pikachu/pikachu_emotions.asm`        | Cry + bubble + movement + portrait + scene callback sequences                 |
| Portrait VM                   | `engine/pikachu/pikachu_pic_animation.asm` | Portrait timing, graphic loading, objects, A/B dismissal, Thunderbolt effect  |
| Portrait programs             | `data/pikachu/pikachu_pic_animation.asm`   | Scripts 0–29                                                                  |
| Portrait frame programs       | `data/pikachu/pikachu_pic_objects.asm`     | Animated tilemap/patch sequences                                              |
| Portrait tilemaps             | `data/pikachu/pikachu_pic_tilemaps.asm`    | Actual 5×5 composition/patch layouts                                          |
| Graphic declarations          | `gfx/pikachu.asm`                          | Maps graphics IDs to source assets                                            |
| PNG source art                | `gfx/pikachu/*.png`                        | Original Partner Pikachu art                                                  |
| Movement interpreter          | `engine/pikachu/pikachu_movement.asm`      | Pikachu-specific movement VM                                                  |
| Happiness/mood                | `engine/events/pikachu_happiness.asm`      | Friendship changes and mood targets                                           |
| Status logic                  | `engine/pikachu/pikachu_status.asm`        | Status/post-battle Partner behavior                                           |
| Trigger hooks                 | item, battle, map scripts                  | Fishing, captures, stones, moves, Bill, Fan Club, etc.                        |

The engine and data directories confirm this subsystem separation directly.

---

# 2. A-button interaction flow

Yellow's direct interaction is:

```text
Player faces Pikachu + presses A
          ↓
IsPlayerTalkingToPikachu
          ↓
InitializePikachuTextID
          ↓
TalkToPikachu
          ↓
MapSpecificPikachuExpression
          │
          ├─ special reaction found → use it
          │
          └─ otherwise
                 ↓
       happiness + mood matrix
                 ↓
          Emotion program
                 ↓
 cry / bubble / movement / portrait / callbacks
```

Yellow's `TalkToPikachu` and `DoStarterPikachuEmotions` provide exactly this structure.

Golden Yellow already has the equivalent **entry point**:

```text
Player presses A on Pokémon follower
          ↓
field_control_avatar.c
          ↓
OBJ_EVENT_ID_FOLLOWER
          ↓
EventScript_Follower
```

Therefore we should extend that existing route for canonical Partner Pikachu instead of modifying global field-input logic. Ordinary followers keep their current interaction behavior.

---

# 3. Reaction precedence

Yellow does not blindly calculate a mood face every time. It first checks higher-priority conditions.

The source order is:

```text
1. Pokémon Fan Club override
2. Pewter Pokémon Center override
3. Bill's House override
4. Partner asleep
5. Other major status condition
6. Pokémon Tower 1F–7F
7. Temporary emotion modifier
8. Normal happiness + mood expression
```

That precedence is directly encoded in `MapSpecificPikachuExpression`.

### Golden Yellow rule

We should reproduce this same hierarchy:

```text
AUTHORED STORY REACTION
        >
STATUS REACTION
        >
AREA REACTION
        >
ONE-SHOT MODIFIER
        >
NORMAL MOOD/HAPPINESS TALK
```

This prevents modern contextual follower messages from overriding important Yellow scenes.

---

# 4. Emotion interpreter catalog

Yellow has an emotion-command interpreter with 11 command IDs.

| ID | Yellow command          | Golden Yellow mapping                   |
| -: | ----------------------- | --------------------------------------- |
|  0 | no-op                   | omit                                    |
|  1 | print text              | script/message callback                 |
|  2 | play Pikachu PCM clip   | Partner cry/audio API                   |
|  3 | show emotion bubble     | GBA field-effect/emote API              |
|  4 | run Pikachu movement    | existing follower movement/choreography |
|  5 | show `pikapic` portrait | generalized accepted portrait renderer  |
|  6 | subcommand              | semantic reaction callback              |
|  7 | delay frames            | task/script delay                       |
|  8 | debug/no-op             | omit production behavior                |
|  9 | turn away from player   | follower-facing action                  |
| 10 | no-op                   | omit                                    |

The exact interpreter jump table and its command handlers are in Yellow's source.

Yellow also has seven subcommands:

| Subcommand | Purpose                            |
| ---------- | ---------------------------------- |
| 0          | load extra Pikachu sprites         |
| 1          | load font                          |
| 2          | restore/show map view              |
| 3          | wait for text button               |
| 4          | Pewter Pokémon Center continuation |
| 5          | Fan Club continuation              |
| 6          | Bill continuation                  |

We should **not port the Game Boy VRAM/font mechanics literally**. Their semantic purpose becomes scene callbacks in GBA.

---

# 5. Complete emotion-program catalog

Yellow defines Emotion IDs **0–33**. These are not identical to portrait IDs.

| Emotion | Bubble / movement | Outer cry | Portrait | Purpose / usage              |
| ------: | ----------------- | --------- | -------: | ---------------------------- |
|       0 | none              | none      |        — | empty                        |
|       1 | —                 | default   |        1 | normal neutral               |
|       2 | Smile             | Cry35     |        2 | happy                        |
|       3 | —                 | Cry40     |        3 | subdued/neutral              |
|       4 | special movement  | Cry29     |        4 | playful/content              |
|       5 | —                 | Cry31     |        5 | displeased                   |
|       6 | movement + Skull  | default   |        6 | irritated                    |
|       7 | repeated movement | Cry1      |        7 | energetic/happy              |
|       8 | —                 | Cry39     |        8 | happy                        |
|       9 | movement + Skull  | Cry6      |        9 | strong displeasure           |
|      10 | Heart             | Cry5      |       10 | affection                    |
|      11 | ZZZ               | Cry37     |       11 | asleep                       |
|      12 | —                 | default   |       12 | displeased                   |
|      13 | movement          | none      |       13 | aloof                        |
|      14 | Bolt              | Cry10     |       14 | highly agitated              |
|      15 | —                 | Cry34     |       15 | happy/content                |
|      16 | —                 | Cry33     |       16 | strong happiness             |
|      17 | —                 | Cry13     |       17 | unhappy/sad                  |
|      18 | —                 | default   |       18 | successful-capture reaction  |
|      19 | Heart             | Cry33     |       19 | strong affection             |
|      20 | Heart             | Cry5      |       20 | maximum affection            |
|      21 | Fish              | default   |       21 | fishing                      |
|      22 | —                 | Cry4      |       22 | Pokémon Tower fear           |
|      23 | —                 | Cry19     |       23 | confused/questioning         |
|      24 | Exclamation       | default   |       24 | Thunder Stone refusal        |
|      25 | Bolt              | Cry35     |       25 | electrical-power reaction    |
|      26 | ZZZ               | Cry37     |       26 | Pewter Jigglypuff sleep/wake |
|      27 | —                 | Cry9      |       27 | restored-Bill shock          |
|      28 | —                 | Cry15     |       28 | sick/status                  |
|      29 | —                 | Cry5      |       10 | Fan Club variant             |
|      30 | turn away + Heart | Cry5      |       20 | stronger Fan Club reaction   |
|      31 | —                 | Cry19     |       23 | Bill post-state variant      |
|      32 | —                 | Cry26     |       23 | Bill intermediate variant    |
|      33 | none              | none      |        — | sentinel/debug empty         |

These command sequences come directly from `data/pikachu/pikachu_emotions.asm`.

The important architectural point is:

> **Emotion ID ≠ portrait ID.**

An emotion is a whole reaction program. A portrait is only one component of it.

---

# 6. Normal happiness + mood system

Yellow's regular A-button expression uses two values:

* `wPikachuHappiness`
* `wPikachuMood`

The mood columns are:

|        Mood | Column |
| ----------: | -----: |
|        0–40 |      1 |
|      41–127 |      2 |
| 128 exactly |      3 |
|     129–210 |      4 |
|     211–255 |      5 |

Then happiness chooses a row:

| Happiness | Very low mood | Low mood | Neutral 128 | Positive | Very positive |
| --------: | ------------: | -------: | ----------: | -------: | ------------: |
|       ≤50 |            14 |       14 |           6 |       13 |            13 |
|      ≤100 |             9 |        9 |           5 |       12 |            12 |
|      ≤130 |             3 |        3 |           1 |        8 |             8 |
|      ≤160 |             3 |        3 |           4 |       15 |            15 |
|      ≤200 |            17 |       17 |           7 |        2 |             2 |
|      ≤250 |            17 |       17 |          16 |       10 |            10 |
|      ≤255 |            17 |       17 |          19 |       20 |            20 |

Those numbers are portrait/emotion selections from Yellow's actual table.

This should be ported **exactly**, not replaced with random expressions.

---

# 7. Happiness rules

Yellow divides current happiness into three bands:

* `<100`
* `100–199`
* `200–255`

Then events change happiness by different amounts.

| Event                              | <100 | 100–199 | ≥200 |
| ---------------------------------- | ---: | ------: | ---: |
| Gain level                         |   +5 |      +3 |   +2 |
| HP restoration/use item            |   +5 |      +3 |   +2 |
| X item                             |   +1 |      +1 |    0 |
| Challenge Gym Leader               |   +3 |      +2 |   +1 |
| Teach TM/HM                        |   +1 |      +1 |    0 |
| Walking                            |   +2 |      +1 |   +1 |
| Deposit Partner                    |   −3 |      −3 |   −5 |
| Faint                              |   −1 |      −1 |   −1 |
| Poison faint outside battle        |   −5 |      −5 |  −10 |
| Faint to ≥30-level-higher opponent |   −5 |      −5 |  −10 |
| Trade                              |  −10 |     −10 |  −20 |

Yellow clamps the result to 0–255.

### Golden Yellow adaptation

We already have Gen III Pokémon friendship.

I recommend:

* use the canonical Partner's actual `MON_DATA_FRIENDSHIP` as the authoritative 0–255 happiness value;
* do **not** create a duplicate Yellow happiness byte;
* add Yellow-specific event adjustments only where our modern friendship implementation does not already produce the intended result.

That keeps Melanie's Bulbasaur requirement and the portrait system reading the same real Partner state.

---

# 8. Mood rules

Mood is separate from happiness.

Yellow's event target values include:

| Cause                          |                        Mood target |
| ------------------------------ | ---------------------------------: |
| Level up                       |                                138 |
| HP restoration                 |                                131 |
| TM/HM category                 | 128 / no effective mood adjustment |
| Gym Leader category            | 128 / no effective mood adjustment |
| one positive internal category |                                148 |
| another neutral category       |                                128 |
| Deposit                        |                                 98 |
| Faint                          |                                108 |
| negative internal category     |                                 98 |
| another negative category      |                                108 |
| extreme negative category      |                                  0 |

A positive target raises mood only if the current mood is lower; a negative target lowers it only if current mood is higher. The special `128` value is treated as a no-change boundary in the source.

This makes mood a short-term emotional state while friendship reflects the long-term relationship.

### Golden Yellow design

This is the one state I **do** recommend preserving separately.

Proposed semantic state:

```c
struct GoldenYellowPartnerState
{
    u8 mood;
    u8 reactionModifier;
};
```

But we should place it in the existing Golden Yellow save/state architecture only after checking padding/versioning constraints. It should **not** be inserted casually into `struct Pokemon`.

Why persistent? Because Yellow mood is gameplay state, and having Pikachu forget its current mood simply because the player saved/reloaded would be undesirable.

---

# 9. One-shot emotion modifiers

Yellow has a separate `wPikachuEmotionModifier`.

The reaction director maps:

| Modifier | Emotion | Known meaning               |
| -------: | ------: | --------------------------- |
|        0 |       — | normal selection            |
|        1 |      18 | successful capture          |
|        2 |      21 | fishing                     |
|        3 |      23 | confused/questioning slot   |
|        4 |      24 | Thunder Stone refusal       |
|        5 |      25 | learned Thunderbolt/Thunder |

The table itself is unambiguous in `MapSpecificPikachuExpression`.

Confirmed source behavior includes learning Thunderbolt or Thunder setting modifier `5` and mood `0x85`.

### Modifier 3 caveat

I have **not found a definitive gameplay writer for modifier 3** in the current `pokeyellow` source audit.

Therefore:

> I cannot confirm a canonical trigger for modifier 3.

We should preserve the slot semantically but **not invent a trigger**.

---

# 10. Portrait program inventory

There are 29 direct runtime portrait IDs, `0–28`.

A nominal pointer for 29 exists, but the loader explicitly converts indices `>=29` back to zero. Scripts 0, 1, and nominal 29 share the same body anyway.

| Portrait | Duration ticks | Primary graphics            | General interpretation   |
| -------: | -------------: | --------------------------- | ------------------------ |
|      0/1 |             40 | `e4000`, `e40cc`            | calm/neutral             |
|        2 |             44 | `e411c`, `e41d2`            | broad happiness          |
|        3 |             80 | `e4272`, `e4323`            | flat/unimpressed         |
|        4 |             70 | `e4383`, `e444b`            | playful/content          |
|        5 |             32 | `e458b`, `e463b`            | sad/displeased           |
|        6 |             50 | `e467b`, `e472e`            | irritated/angry          |
|        7 |             58 | `e476e`, `e4841`            | energetic happiness      |
|        8 |             44 | `e49d1`, `e4a99`            | happy grin               |
|        9 |             56 | `e4b39`, `e4bde`            | strong dislike           |
|       10 |             56 | `e4c3e`, `e4ce0`, `e4e70`   | affection/hearts         |
|       11 |            100 | `e5000`, `e50af`            | sleeping                 |
|       12 |             50 | `e523f`, `e52fe`            | frowning                 |
|       13 |             50 | `e548e`, `e5541`            | aloof/annoyed            |
|       14 |             40 | `e56d1`, `e5794`            | very angry               |
|       15 |             50 | `e5924`, `e59ed`            | content                  |
|       16 |             32 | `e5b7d`, `e5c4d`            | strong happiness         |
|       17 |            100 | `e5ddd`, `e5e90`            | unhappy                  |
|       18 |             32 | `e6020`, `e61b0`            | catch victory/V-sign     |
|       19 |             44 | `e6340`, `e63f7`            | strong affection         |
|       20 |             50 | `e6587`, `e6646`            | maximum affection        |
|       21 |             40 | `e67d6` + 4 patches         | fishing                  |
|       22 |             40 | `e6e6f`, `e6fff`            | frightened/Tower         |
|       23 |             70 | `e718f`, `e731f`            | confused/questioning     |
|       24 |             60 | `e74af`, `e763f`            | refusal/head shake       |
|       25 |             50 | `e77cf`,`e7863`,`e79f3`     | electrical/Thunderbolt   |
|       26 |            100 | sleep art + `e7b83`,`e7d13` | Jigglypuff wake sequence |
|       27 |             30 | `f0abf`,`f0b64`             | shocked Bill reaction    |
|       28 |             64 | `f0cf4`,`f0d82`             | sick/pained              |

The duration and graphic assignments are source-exact.

Remember: each Yellow portrait interpreter tick contains a `Delay3`, so the visible GBA-equivalent lifetime is approximately:

```text
duration × 3 frames
```

That is why our accepted Bill mapping used:

* 70 → 210 frames
* 30 → 90 frames.

---

# 11. Full Yellow graphic asset manifest

The portrait system references **61 Pikachu graphic components**, plus one overworld `PikachuSprite` resource in a separate setup program.

The source filenames are:

```text
unknown_e4000
unknown_e40cc
unknown_e411c
unknown_e41d2
unknown_e4272
unknown_e4323
unknown_e4383
unknown_e444b
unknown_e458b
unknown_e463b
unknown_e467b
unknown_e472e
unknown_e476e
unknown_e4841
unknown_e49d1
unknown_e4a99
unknown_e4b39
unknown_e4bde
unknown_e4c3e
unknown_e4ce0
unknown_e4e70
unknown_e5000
unknown_e50af
unknown_e523f
unknown_e52fe
unknown_e548e
unknown_e5541
unknown_e56d1
unknown_e5794
unknown_e5924
unknown_e59ed
unknown_e5b7d
unknown_e5c4d
unknown_e5ddd
unknown_e5e90
unknown_e6020
unknown_e61b0
unknown_e6340
unknown_e63f7
unknown_e6587
unknown_e6646
unknown_e67d6
unknown_e682f
unknown_e69bf
unknown_e6b4f
unknown_e6cdf
unknown_e6e6f
unknown_e6fff
unknown_e718f
unknown_e731f
unknown_e74af
unknown_e763f
unknown_e77cf
unknown_e7863
unknown_e79f3
unknown_e7b83
unknown_e7d13
unknown_f0abf
unknown_f0b64
unknown_f0cf4
unknown_f0d82
```

`gfx/pikachu.asm` establishes the exact mapping between these source files and portrait GFX IDs.

### Import rule

We should import the original PNG source art, but **not the Game Boy `.pic`/`.2bpp` storage format**.

Our conversion pipeline should produce GBA-native indexed 4bpp carriers while preserving the original pixel art exactly.

---

# 12. Portrait VM commands

The Yellow portrait engine itself has 15 command opcodes.

| Opcode | Function                       |
| -----: | ------------------------------ |
|   `00` | no-op                          |
|   `01` | set interpreter delay byte     |
|   `02` | load graphic                   |
|   `03` | create animation object        |
|   `04` | no-op                          |
|   `05` | no-op                          |
|   `06` | delete animation object        |
|   `07` | no-op                          |
|   `08` | no-op                          |
|   `09` | jump                           |
|   `0A` | set total portrait duration    |
|   `0B` | play Pikachu cry               |
|   `0C` | Thunderbolt audiovisual effect |
|   `0D` | yield/run current animation    |
|   `0E` | terminate portrait             |

The runtime jump table defines these explicitly.

The Thunderbolt command is significant: it temporarily pauses/mutes normal audio, plays Thunderbolt sound, and repeatedly flashes the palette.

### Golden Yellow translation

We should **not build a byte-for-byte GB portrait bytecode interpreter**.

Instead convert each Yellow program into a declarative GBA descriptor:

```c
struct PartnerPortraitDef
{
    const struct PartnerPortraitFrame *frames;
    u16 lifetimeFrames;
    u8 sourceScriptId;
    u8 flags;
    u16 internalCry;
    void (*specialEffect)(...);
};
```

This keeps source fidelity without dragging Game Boy VRAM architecture into GBA.

---

# 13. Lower-level animation timing

The portrait scripts do considerably more than switch once between two images.

Yellow's object programs contain timed partial-image changes. Examples:

* Script 10 cycles several heart/affection patches.
* Script 21 builds the fishing/bucket sequence across multiple graphics.
* Script 26 transitions through sleeping and wake-up art.
* Script 25 culminates in a full Thunderbolt flash effect.
* Bill Script 23 swaps to the second confused frame after 16 interpreter ticks.
* Bill Script 27 swaps to the radiating shock frame after 4 ticks.

The complete frame programs are contained in `PikaPicAnimBGFrames_0–35`.

### Translation rule

We should reproduce those **visible timelines**, not the literal Yellow tilemap machinery.

For every portrait we need to flatten:

```text
Yellow base graphic
+
Yellow tile patches
+
Yellow tilemap state at time T
              ↓
complete rendered 40×40 frame
              ↓
64×64 GBA carrier
```

That gives us deterministic full frames and greatly simplifies VRAM/resource handling.

This is important: simply importing the two obvious PNGs for each portrait would **not** fully reproduce scripts such as 10, 21, 25, or 26.

---

# 14. Movement subsystem

Yellow's Pikachu reaction movement is its own interpreter.

It supports, among other things:

* directional steps;
* slides;
* hops;
* face directions;
* relative movement based on current facing;
* clockwise/counterclockwise stepping;
* diagonal movement;
* delays;
* special animation/shadow behavior.

The movement database explicitly defines:

* `$1D–20`: step down/up/left/right
* `$21–24`: diagonal steps
* `$25–28`: slide
* `$29–2C`: diagonal slide
* `$2D–30`: hop
* `$31–34`: diagonal hop
* `$35–38`: face direction
* `$39–3E`: parameterized special/delay animation commands
* `$3F`: end.

### Golden Yellow mapping

Do **not** import this movement VM wholesale.

We already have a much stronger GBA follower/movement engine.

Translate each used Yellow reaction movement into:

* GBA movement scripts;
* follower movement helpers;
* `face_player`;
* safe local positioning;
* emote field effects.

Only create a new movement primitive if Yellow visibly requires something our engine genuinely cannot represent.

---

# 15. Emotion bubbles

Yellow uses these reaction bubbles in the Pikachu subsystem:

| Bubble      | Uses                     |
| ----------- | ------------------------ |
| Smile       | happy reaction           |
| Heart       | affection/Fan Club       |
| Skull       | strong dislike/agitation |
| ZZZ         | sleeping/Jigglypuff      |
| Bolt        | angry/electric reaction  |
| Fish        | fishing                  |
| Exclamation | refusal                  |
| Question    | Bill scene choreography  |

The Bill question bubble is scene choreography rather than part of Emotion23 itself. Bill's source sequence moves Pikachu, displays the question mark, disables following, then initiates Pikachu interaction.

### GBA rule

Reuse existing field-effect bubbles where equivalent.

Only the Yellow-specific **Fish** graphic should get a custom field-effect asset if the expansion does not already provide one.

---

# 16. Pikachu cries

There are two audio layers:

### Emotion-program cries

Confirmed explicit emotion-level clips include:

```text
Cry1
Cry4
Cry5
Cry6
Cry9
Cry10
Cry13
Cry15
Cry19
Cry26
Cry29
Cry31
Cry33
Cry34
Cry35
Cry37
Cry39
Cry40
```

Some emotion commands use the default/no-explicit-clip form.

### Portrait-internal cries

Certain portrait scripts also specify their own internal clip:

| Portrait | Internal cry |
| -------: | ------------ |
|      0/1 | Cry3         |
|        6 | Cry38        |
|       12 | Cry25        |
|       18 | Cry18        |
|       21 | Cry20        |

Others use the generic `pikapic_cry` behavior or no explicit special clip.

### Audio implementation recommendation

For Phase 1 of the system:

* preserve the current Gen III Partner Pikachu cry as fallback;
* build the reaction engine so a reaction can request a semantic Yellow cry ID;
* import the actual Yellow PCM clips in a separate controlled audio pass.

This prevented audio conversion from blocking the P1–P8 graphics/reaction architecture.

**Current P9 implementation:** the earlier generic-fallback recommendation is superseded for Yellow-owned commands by the locked P9A reachability audit. P9B preserves stable IDs `01–42`, imports only the 27 Required clips initially, treats Yellow `$FF` as true `NO_PCM`, keeps ordinary Pikachu behavior unchanged, and preserves separate outer-Emotion and portrait-internal cry timing.

The imported PCM is filtered for GBA playback and divided into quiet, balanced, and naturally loud energy groups. The accepted working runtime levels are `115 / 95 / 76`. Yellow PCM now overlaps the current BGM without using the ordinary cry duck/restore task, and the reaction director waits for the actual Yellow PCM player to finish. These audio rules are protected while the reported reaction defects are investigated.

---

# 17. Major interaction triggers

The source audit identifies these classes of trigger.

## Normal direct interaction

Player talks directly to following Partner Pikachu.

Result:

```text
special override?
otherwise friendship + mood portrait
```

This becomes the primary new feature.

---

## Bill

Source-specific states:

* transformed Bill → Emotion23
* restored human immediate state → Emotion27
* later Bill states → Emotion31/32, which reuse Portrait23.

Bill's exact mapping comes from `BillsHouse_CheckPikachuEmotion`.

Our accepted Golden Yellow implementation already proves the two important cinematic portraits.

---

## Pewter Jigglypuff

The Pewter Pokémon Center has a map-specific Partner state and interaction path.

The full Yellow-specific direct-talk reaction is Emotion26:

```text
ZZZ
Cry37
Portrait26
restore map
Pewter-specific continuation
```

Our current accepted Golden Yellow Jigglypuff scene already gives us the overworld choreography foundation; the full portrait system can enhance it later without reopening the accepted story result.

---

## Pokémon Fan Club

The Fan Club has a dedicated Partner script:

* verifies starter Pikachu;
* rejects special choreography if Pikachu has a status condition;
* displays an exclamation;
* moves Pikachu toward the club Pokémon;
* initiates Pikachu interaction;
* temporarily disables normal following.

Its portrait variants are:

* Emotion29 → Portrait10
* Emotion30 → Portrait20.

---

## Pokémon Tower

Any direct Partner interaction on Tower floors 1F–7F chooses:

```text
Emotion22 → Portrait22
```

before generic mood behavior.

---

## Sleep/status

* sleep → Emotion11 / Portrait11
* other status → Emotion28 / Portrait28.

Again, these outrank mood.

---

## Capture

A successful capture can set the one-shot celebration reaction:

```text
modifier 1
→ Emotion18
→ victory/V-sign portrait
```

---

## Fishing

Fishing sets:

```text
modifier 2
→ Emotion21
→ FISH bubble + fishing portrait
```

---

## Thunder Stone

Trying to evolve canonical Partner Pikachu produces:

```text
refusal
→ modifier 4
→ Emotion24
→ exclamation + head-shake portrait
```

This should integrate directly with our already-required Partner evolution lock.

---

## Learning Thunderbolt / Thunder

When canonical Partner learns either move:

```text
modifier 5
mood 133
→ Emotion25
→ electrical portrait + Thunderbolt audiovisual effect
```

The level-up source explicitly performs that assignment.

---

# 18. Golden Yellow target architecture

I recommend these production modules.

```text
┌───────────────────────────────────────────────┐
│ EXISTING FOLLOWER ENGINE                     │
│ movement / warps / sprite / collision        │
└───────────────────────┬───────────────────────┘
                        │ A-button
                        ▼
             Is actual follower Partner?
               /                   \
             No                     Yes
             │                       │
 existing follower messages          ▼
                         ┌────────────────────────┐
                         │ PARTNER REACTION       │
                         │ DIRECTOR               │
                         └───────────┬────────────┘
                                     │
                 ┌───────────────────┼──────────────────┐
                 │                   │                  │
             story override       status           normal talk
                 │                   │                  │
                 └───────────────────┼──────────────────┘
                                     ▼
                           happiness + mood
                              where needed
                                     │
                                     ▼
                         REACTION PROGRAM TABLE
                                     │
                 ┌───────────────────┼──────────────────┐
                 │                   │                  │
              portrait            cry/emote          movement
                 │                   │                  │
                 ▼                   ▼                  ▼
        PORTRAIT RENDERER     GBA audio/field FX   follower APIs
```

---

# 19. Recommended source layout

I would restructure the current Bill prototype into something like:

```text
include/
  golden_yellow_partner_reaction.h
  constants/golden_yellow_partner_reactions.h

src/
  golden_yellow_partner_reaction.c
  golden_yellow_partner_portrait.c
  golden_yellow_partner_state.c

src/data/golden_yellow/
  partner_reactions.h
  partner_portraits.h
  partner_mood_table.h

graphics/golden_yellow/pikachu_portraits/
  00_neutral_*.png
  02_happy_*.png
  ...
  bill_confused_*.png
  bill_shocked_*.png
```

I would retain source comments such as:

```text
Yellow source:
PikaPicAnimScript23
GFX_e718f
GFX_e731f
```

so every converted asset remains traceable.

---

# 20. Semantic reaction IDs

Map scripts and engine hooks should **not** use raw Yellow numbers.

Use semantic constants such as:

```c
PARTNER_REACTION_NORMAL
PARTNER_REACTION_SLEEPING
PARTNER_REACTION_STATUS_SICK
PARTNER_REACTION_CAPTURE_SUCCESS
PARTNER_REACTION_FISHING
PARTNER_REACTION_TOWER_AFRAID
PARTNER_REACTION_THUNDER_STONE_REFUSAL
PARTNER_REACTION_ELECTRIC_POWER
PARTNER_REACTION_PEWTER_JIGGLYPUFF
PARTNER_REACTION_BILL_CONFUSED
PARTNER_REACTION_BILL_SHOCKED
PARTNER_REACTION_FAN_CLUB_AFFECTION
PARTNER_REACTION_FAN_CLUB_MAX_AFFECTION
```

Each entry can preserve:

```c
.sourceEmotionId = 27,
.sourcePortraitId = 27,
```

for provenance.

That gives us readable Golden Yellow code without losing exact Yellow traceability.

---

# 21. A-button integration design

Current:

```asm
EventScript_Follower:
    lock
    bufferlivemonnickname STR_VAR_1
    playfirstmoncry
    getfolloweraction
    ...
```

Target conceptually becomes:

```text
EventScript_Follower
       ↓
resolve actual following Pokémon
       ↓
canonical Partner Pikachu?
       │
       ├─ yes → GoldenYellow Partner Talk
       │
       └─ no  → existing getfolloweraction
```

Crucially:

> **Do not identify Partner merely by graphics ID or `SPECIES_PIKACHU`.**

Use the existing Partner-aware follower identity architecture.

---

# 22. Portrait renderer evolution

The accepted Bill renderer should become generalized rather than replaced.

Preserve:

* `CreateSpriteUnchecked`
* explicit task-capacity guard
* native 64×64 OBJ
* centered window
* shared grayscale palette
* script-blocking native wait
* A/B dismissal
* deterministic cleanup
* no follower state mutation.

Remove Bill-specific assumptions:

```text
hard-coded two reactions
hard-coded four source graphics
Bill-specific lifetime selection
Bill-specific animation table
```

Replace them with resource descriptors.

---

# 23. Frame-generation strategy

This is important enough to lock now.

Rather than implementing Yellow's patch/tilemap compositor at runtime on GBA:

### Build-time conversion

For every portrait:

1. load Yellow source PNG components;
2. reconstruct every visible 40×40 state from the Yellow tilemap program;
3. emit complete indexed PNG frames;
4. center them inside 64×64 carriers;
5. compile as normal GBA 4bpp sprite frames.

Advantages:

* exact visual fidelity;
* much simpler runtime;
* predictable OBJ VRAM;
* easier testing;
* no need to reproduce Yellow's dangerous eight-GFX VRAM allocator;
* easier future replacement with upgraded Gen III-style art.

Yellow's own renderer even contains a warning that exceeding its tiny graphic bookkeeping can execute arbitrary code—exactly the sort of GB implementation detail we should **not** recreate.

---

# 24. Modern follower-context integration

The current follower system already has contextual reactions for:

* weather;
* terrain;
* map;
* time of day;
* status;
* environment;
* emotional categories.

We should retain that infrastructure.

For canonical Partner Pikachu:

```text
Yellow authored override exists?
        YES → Yellow owns reaction
        NO
        ↓
Yellow normal portrait from friendship + mood
        ↓
optional modern contextual follower text/choreography
```

That creates a richer result than original Yellow while preserving its personality model.

Example:

```text
Player talks to Pikachu in rain
→ mood portrait opens
→ portrait closes
→ contextual “Pikachu seems happy about the rain” behavior may run
```

But:

```text
Player talks in Pokémon Tower
→ Tower fear reaction
→ NO random rain/terrain text replacing it
```

---

# 25. Implementation stages

I would implement this in controlled stages.

### Stage P1 — Complete portrait asset conversion

* import all 61 source PNG components;
* reconstruct all final visual frames;
* build portrait descriptor table;
* preserve source IDs;
* no gameplay hooks yet.

**Acceptance:** debug viewer can display every portrait animation correctly.

### Stage P2 — General portrait engine

Refactor Bill renderer into reusable system.

**Acceptance:** all 0–28 programs render, animate, dismiss, and clean up safely.

### Stage P3 — Reaction program director

Implement:

* emotion descriptors;
* cry;
* bubbles;
* follower-facing/movement callbacks;
* portrait requests;
* delays.

**Acceptance:** manually invoke every Emotion0–32 in debug mode.

### Stage P4 — Normal A-button Partner interaction

Hook canonical Partner talk into `EventScript_Follower`.

Add exact 7×5 matrix.

**Acceptance:** different friendship/mood combinations select the correct source portrait.

### Stage P5 — Mood lifecycle

Connect:

* level ups;
* healing;
* walking;
* Gym Leaders;
* fainting;
* appropriate negative/positive state changes.

**Acceptance:** mood moves through Yellow thresholds deterministically.

### Stage P6 — One-shot modifiers

Integrate:

* capture;
* fishing;
* Thunder Stone;
* Thunderbolt/Thunder.

**Acceptance:** one-time reaction appears at correct next Partner interaction and does not repeat indefinitely.

### Stage P7 — Required authored scenes

Integrate/upgrade:

* Pewter Jigglypuff;
* Bill — preserve accepted result;
* Fan Club;
* Pokémon Tower.

**Acceptance:** scene-specific reactions override generic mood exactly as Yellow dictates.

### Stage P8 — Modern follower coexistence

Layer existing contextual follower messages underneath the Yellow priority system.

**Status:** ACCEPTED on the `4da0f138` implementation plus `298dd506` specific-context fix.

**Acceptance:** ordinary followers remain unchanged and Partner receives both systems without contradictory reactions.

### Stage P9 — Audio fidelity

P9 is split into controlled subpasses:

- **P9A — LOCKED:** source/call-site reachability audit and Option B inclusion policy;
- **P9B — ACCEPTED FOR CURRENT AUDIO SCOPE:** 27 Required clips, filtered PCM, tiered gain, true `NO_PCM`, and non-ducking BGM overlay;
- **P9 closeout — ACTIVE:** correct the newly reported Partner reaction defects and complete targeted regression validation;
- later P9 extension passes may wire deferred system-specific call sites only when their corresponding gameplay becomes applicable.

Do not import all 42 clips merely because the canonical namespace contains 42 IDs.

**Acceptance:** appropriate audio IDs and `NO_PCM` behavior match the locked source/call-site table without regressing ordinary Pikachu or P1–P8 Partner behavior.

---

# 26. Regression rules

These should be non-negotiable while implementing:

1. `SPECIES_PIKACHU_STARTER` remains the only canonical Partner.
2. Ordinary Pikachu never receives Yellow Partner-only reactions.
3. Existing follower engine remains the movement/warp authority.
4. Accepted Bill choreography must not be redesigned while generalizing its renderer.
5. Accepted Pewter Jigglypuff scene must not regress.
6. Portraits cannot leak OBJ tiles, palettes, windows, or tasks.
7. Portrait interaction must block field movement safely.
8. A/B must dismiss normal portraits just as Yellow does.
9. Long Yellow durations cannot be placed blindly into `ANIMCMD_FRAME`; GBA's duration field is too small.
10. No raw Game Boy VRAM architecture should be transplanted merely for source similarity.
11. Story reactions outrank modern generic contextual follower behavior.
12. Save-state changes must respect existing project save architecture and physical GBA compatibility.

---

# 27. What we can preserve almost exactly

These parts should be source-faithful:

* all original art;
* portrait timing;
* frame sequence;
* happiness thresholds;
* mood thresholds;
* 7×5 selection matrix;
* reaction precedence;
* Emotion→Portrait mappings;
* special-event mappings;
* one-shot reaction meanings;
* A/B dismissal;
* visible bubbles;
* visible movement intent;
* Thunderbolt flash presentation;
* Partner-specific status reactions.

---

# 28. What should be translated rather than literally ported

These Yellow internals should **not** be copied:

| Game Boy implementation        | GBA replacement                        |
| ------------------------------ | -------------------------------------- |
| 2bpp dynamic portrait graphics | static GBA 4bpp frames                 |
| GB BG 5×5 patch renderer       | complete pre-rendered OBJ frames       |
| GB Pikachu movement VM         | existing GBA follower/movement scripts |
| manual map-view reloads        | standard GBA window/sprite cleanup     |
| Yellow sprite state swapping   | follower object helpers                |
| raw PCM engine assumptions     | GBA sound API                          |
| Yellow text engine/font reload | existing script/message system         |
| GB save-memory layout          | Golden Yellow state/save architecture  |

This follows our project's core rule: **preserve behavior, translate implementation.**

---

# 29. Remaining uncertainties

The audit was able to establish the complete visible reaction/portrait architecture, but there are two points I would keep explicitly unresolved rather than invent answers:

### Modifier 3

The reaction table clearly maps modifier `3` to Emotion23, but I have not confirmed its writer.

**Status:** preserve the slot; do not implement a trigger until sourced.

### Exact initial/default mood initialization

The selector and every known mood mutation are clear, but I have not yet found a source location that conclusively establishes the initial runtime value in a way I would be comfortable treating as canonical.

**Status:** do not guess. Before P5 we should trace new-game/Oak-Lab initialization and Yellow's save-memory lifecycle specifically.

Neither uncertainty blocks the asset import, general renderer, reaction descriptors, or A-button architecture.

---

# Final blueprint decision

The full system should therefore be:

> **Golden Yellow's existing Pokémon follower engine + canonical Partner resolver + a source-faithful Yellow Partner Reaction Director + generalized Yellow portrait renderer + real Partner friendship + Yellow-style mood/transient reaction state.**

That gives us all of the original Yellow behavior while keeping the implementation native to `pokeemerald-expansion`/GBA.

The Bill work is the correct prototype for this system—not something we need to redo. We generalize it.

And the most useful first implementation pass is **P1 + P2: import/reconstruct the complete Yellow portrait asset library and turn the accepted Bill renderer into the generic portrait engine**, with a debug/test entry point that lets us cycle through all 29 source portrait programs before connecting A-button gameplay.
