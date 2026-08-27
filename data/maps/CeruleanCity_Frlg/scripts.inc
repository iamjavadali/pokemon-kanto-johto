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

.include "data/maps/CeruleanCity_Frlg/scripts_frlg_base.inc"

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
