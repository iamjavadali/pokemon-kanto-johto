@ Golden Yellow Route 22 rival wrapper.
.include "data/scripts/yellow_rival_dispatch_begin.inc"

.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    .ifc \text,Route22_Text_EarlyRivalIntro
        loadword 0, Route22_Text_YellowEarlyIntro
        callstd \type
    .else
        .ifc \text,Route22_Text_EarlyRivalPostBattle
            loadword 0, Route22_Text_YellowEarlyPostBattle
            callstd \type
        .else
            .ifc \text,Route22_Text_LateRivalIntro
                loadword 0, Route22_Text_YellowLateIntro
                callstd \type
            .else
                .ifc \text,Route22_Text_LateRivalPostBattle
                    loadword 0, Route22_Text_YellowLatePostBattle
                    callstd \type
                .else
                    loadword 0, \text
                    callstd \type
                .endif
            .endif
        .endif
    .endif
.endm

.purgem trainerbattle_earlyrival
.macro trainerbattle_earlyrival trainer:req, flags:req, lose_text:req, victory_text:req
    .if (\trainer >= TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_ROUTE22_EARLY_CHARMANDER)
        trainerbattle TRAINER_BATTLE_EARLY_RIVAL, OBJ_ID_NONE, \trainer, NULL, Route22_Text_YellowEarlyDefeat, NULL, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, Route22_Text_YellowEarlyVictory, NULL, FALSE, TRUE, FALSE, FALSE, \flags
        @ Yellow: only a win here can promote the provisional Flareon lane to Jolteon.
        @ A loss leaves Flareon/Vaporeon unchanged. UNSET is a development-save fallback.
        goto_if_eq VAR_RESULT, TRUE, .LYellowRivalRoute22StateDone\@
        goto_if_eq VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_VAPOREON, .LYellowRivalRoute22StateDone\@
        setvar VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_JOLTEON
.LYellowRivalRoute22StateDone\@:
    .else
        trainerbattle TRAINER_BATTLE_EARLY_RIVAL, OBJ_ID_NONE, \trainer, NULL, \lose_text, NULL, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, \victory_text, NULL, FALSE, TRUE, FALSE, FALSE, \flags
    .endif
.endm

.purgem trainerbattle_no_intro
.macro trainerbattle_no_intro trainer:req, lose_text:req
    .if (\trainer >= TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER)
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, Route22_Text_YellowLateDefeat, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, Route22_Text_YellowLateVictory, NULL, FALSE, TRUE, FALSE, FALSE
    .else
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, \lose_text, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
    .endif
.endm

.include "data/maps/Route22_Frlg/scripts_frlg_base.inc"

Route22_Text_YellowEarlyIntro::
    .string "{RIVAL}: Hey!\n"
    .string "{PLAYER}!\p"
    .string "You're going to POKéMON LEAGUE?\p"
    .string "Forget it! You probably don't\n"
    .string "have any BADGES!\p"
    .string "The guard won't let you through!\p"
    .string "By the way, did your POKéMON\n"
    .string "get any stronger?$"

Route22_Text_YellowEarlyDefeat::
    .string "Awww!\n"
    .string "You just lucked out!$"

Route22_Text_YellowEarlyVictory::
    .string "{RIVAL}: What?\n"
    .string "Why do I have 2 POKéMON?\p"
    .string "You should catch some more too!$"

Route22_Text_YellowEarlyPostBattle::
    .string "I heard POKéMON LEAGUE has many\n"
    .string "tough TRAINERS!\p"
    .string "I have to figure out how to get\n"
    .string "past them!\p"
    .string "You should quit dawdling and get\n"
    .string "a move on!$"

Route22_Text_YellowLateIntro::
    .string "{RIVAL}: What?\n"
    .string "{PLAYER}! What a surprise to see\n"
    .string "you here!\p"
    .string "So you're going to POKéMON LEAGUE?\p"
    .string "You collected all the BADGES too?\n"
    .string "That's cool!\p"
    .string "Then I'll whip you, {PLAYER}, as a\n"
    .string "warmup for POKéMON LEAGUE!\p"
    .string "Come on!$"

Route22_Text_YellowLateDefeat::
    .string "What!?\p"
    .string "I was just careless!$"

Route22_Text_YellowLateVictory::
    .string "{RIVAL}: Hahaha!\n"
    .string "{PLAYER}! That's your best? You're\n"
    .string "nowhere near as good as me, pal!\p"
    .string "Go train some more! You loser!$"

Route22_Text_YellowLatePostBattle::
    .string "That loosened me up! I'm ready for\n"
    .string "POKéMON LEAGUE!\p"
    .string "{PLAYER}, you need more practice!\p"
    .string "But hey, you know that! I'm out of\n"
    .string "here. Smell ya!$"

.purgem trainerbattle_earlyrival
.macro trainerbattle_earlyrival trainer:req, flags:req, lose_text:req, victory_text:req
    trainerbattle TRAINER_BATTLE_EARLY_RIVAL, OBJ_ID_NONE, \trainer, NULL, \lose_text, NULL, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, \victory_text, NULL, FALSE, TRUE, FALSE, FALSE, \flags
.endm

.purgem trainerbattle_no_intro
.macro trainerbattle_no_intro trainer:req, lose_text:req
    trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, \lose_text, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
.endm

.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    loadword 0, \text
    callstd \type
.endm

.include "data/scripts/yellow_rival_dispatch_end.inc"
