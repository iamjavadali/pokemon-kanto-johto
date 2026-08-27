@ Golden Yellow Cerulean rival wrapper.
@ Preserve FRLG movement/geometry but route the battle through Yellow's Eevee
@ state, restore Yellow dialogue, and retain FireRed's Fame Checker handoff afterward.
.include "data/scripts/yellow_rival_dispatch_begin.inc"

.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    .ifc \text,CeruleanCity_Text_RivalIntro
        loadword 0, CeruleanCity_Text_YellowRivalIntro
        callstd \type
    .else
        .ifc \text,CeruleanCity_Text_RivalPostBattle
            loadword 0, CeruleanCity_Text_YellowRivalPostBattle
            callstd \type
        .else
            loadword 0, \text
            callstd \type
        .endif
    .endif
.endm

.purgem trainerbattle_no_intro
.macro trainerbattle_no_intro trainer:req, lose_text:req
    .if (\trainer >= TRAINER_RIVAL_CERULEAN_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_CERULEAN_CHARMANDER)
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, CeruleanCity_Text_YellowRivalDefeat, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, CeruleanCity_Text_YellowRivalVictory, NULL, FALSE, TRUE, FALSE, FALSE
    .else
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, \lose_text, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
    .endif
.endm

@ The FRLG base now contains generic post-battle follower staging hooks. Golden Yellow
@ suppresses those immediate calls so Partner Pikachu remains directly behind the
@ player until Blue has completed his first departure. The exit calls are redirected
@ to split choreography below, where Pikachu steps onto a known walkable trigger tile
@ only as Blue begins walking back for the Fame Checker.
.purgem call_if_eq
.macro call_if_eq a:req, b, c
    .ifnb \c
        .ifc \c,CeruleanCity_EventScript_PartnerStepAsideLeft
        .else
            .ifc \c,CeruleanCity_EventScript_PartnerStepAsideRight
            .else
                .ifc \c,CeruleanCity_EventScript_RivalStartExit
                    compare \a, \b
                    call_if EQUAL, CeruleanCity_EventScript_YellowRivalStartExit
                .else
                    .ifc \c,CeruleanCity_EventScript_RivalStartExitRight
                        compare \a, \b
                        call_if EQUAL, CeruleanCity_EventScript_YellowRivalStartExitRight
                    .else
                        trycompare call_if, EQUAL, \a, \b, \c
                    .endif
                .endif
            .endif
        .endif
    .else
        trycompare call_if, EQUAL, \a, \b, \c
    .endif
.endm

.include "data/maps/CeruleanCity_Frlg/scripts_frlg_base.inc"

@ Restore the standard comparison helper before defining Golden Yellow additions or
@ assembling the next map script.
.purgem call_if_eq
.macro call_if_eq a:req, b, c
    trycompare call_if, EQUAL, \a, \b, \c
.endm

CeruleanCity_EventScript_YellowRivalStartExit::
    applymovement LOCALID_PLAYER, CeruleanCity_Movement_PlayerWatchRivalExit
    applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_YellowRivalDepart
    waitmovement LOCALID_CERULEAN_RIVAL
    applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_YellowRivalShiftToReturnLane
    waitmovement LOCALID_CERULEAN_RIVAL
    applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_YellowRivalReturn
    compare VAR_TEMP_1, 0
    call_if EQUAL, CeruleanCity_EventScript_YellowPartnerStepAsideRight
    compare VAR_TEMP_1, 1
    call_if EQUAL, CeruleanCity_EventScript_YellowPartnerStepAsideLeft
    waitmovement 0
    return

CeruleanCity_EventScript_YellowRivalStartExitRight::
    applymovement LOCALID_PLAYER, CeruleanCity_Movement_PlayerWatchRivalExitRight
    applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_YellowRivalDepartRight
    waitmovement LOCALID_CERULEAN_RIVAL
    applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_YellowRivalShiftToReturnLaneRight
    waitmovement LOCALID_CERULEAN_RIVAL
    applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_YellowRivalReturn
    applymovement OBJ_EVENT_ID_FOLLOWER, CeruleanCity_Movement_PartnerStepAsideLeft
    waitmovement 0
    return

CeruleanCity_EventScript_YellowPartnerStepAsideLeft::
    applymovement OBJ_EVENT_ID_FOLLOWER, CeruleanCity_Movement_PartnerStepAsideLeft
    return

CeruleanCity_EventScript_YellowPartnerStepAsideRight::
    applymovement OBJ_EVENT_ID_FOLLOWER, CeruleanCity_Movement_PartnerStepAsideRight
    return

@ Phase 1: Blue clears the player/follower column and runs south.
CeruleanCity_Movement_YellowRivalDepart::
    walk_right
    walk_down
    walk_down
    walk_down
    walk_down
    walk_down
    walk_down
    walk_down
    delay_16
    delay_16
    step_end

CeruleanCity_Movement_YellowRivalDepartRight::
    walk_left
    walk_down
    walk_down
    walk_down
    walk_down
    walk_down
    walk_down
    walk_down
    delay_16
    delay_16
    step_end

@ Phase 2: Blue shifts back into the player's column at the south end.
CeruleanCity_Movement_YellowRivalShiftToReturnLane::
    walk_left
    step_end

CeruleanCity_Movement_YellowRivalShiftToReturnLaneRight::
    walk_right
    step_end

@ Phase 3: Blue walks north while Partner Pikachu moves off the return tile.
CeruleanCity_Movement_YellowRivalReturn::
    walk_up
    walk_up
    walk_up
    walk_up
    walk_up
    step_end

CeruleanCity_Text_YellowRivalIntro::
    .string "{RIVAL}: Yo!\n"
    .string "{PLAYER}!\p"
    .string "You're still struggling along back\n"
    .string "here?\p"
    .string "I'm doing great! I caught a bunch\n"
    .string "of strong and smart POKéMON!\p"
    .string "Here, let me see what you caught,\n"
    .string "{PLAYER}!$"

CeruleanCity_Text_YellowRivalDefeat::
    .string "Hey!\n"
    .string "Take it easy!\l"
    .string "You won already!$"

CeruleanCity_Text_YellowRivalVictory::
    .string "Heh!\n"
    .string "You're no match for my genius!$"

CeruleanCity_Text_YellowRivalPostBattle::
    .string "{RIVAL}: Hey, guess what?\p"
    .string "I went to BILL's and got him to\n"
    .string "show me his rare POKéMON!\p"
    .string "That added a lot of pages to my\n"
    .string "POKéDEX!\p"
    .string "After all, BILL's world famous as a\n"
    .string "POKéMANIAC!\p"
    .string "He invented the POKéMON Storage\n"
    .string "System on PC!\p"
    .string "Since you're using his system, go\n"
    .string "thank him!\p"
    .string "Well, I better get rolling!\n"
    .string "Smell ya later!$"

@ Restore standard event macros before the next map script is assembled.
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
