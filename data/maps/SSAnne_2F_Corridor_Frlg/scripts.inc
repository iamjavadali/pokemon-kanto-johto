@ Golden Yellow S.S. Anne rival wrapper.
@ Preserve FRLG choreography while restoring Yellow's team branch and dialogue.
.include "data/scripts/yellow_rival_dispatch_begin.inc"

.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    .ifc \text,SSAnne_2F_Corridor_Text_RivalIntro
        loadword 0, SSAnne_2F_Corridor_Text_YellowRivalIntro
        callstd \type
    .else
        .ifc \text,SSAnne_2F_Corridor_Text_RivalPostBattle
            loadword 0, SSAnne_2F_Corridor_Text_YellowRivalPostBattle
            callstd \type
        .else
            loadword 0, \text
            callstd \type
        .endif
    .endif
.endm

.purgem trainerbattle_no_intro
.macro trainerbattle_no_intro trainer:req, lose_text:req
    .if (\trainer >= TRAINER_RIVAL_SS_ANNE_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_SS_ANNE_CHARMANDER)
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, SSAnne_2F_Corridor_Text_YellowRivalDefeat, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, SSAnne_2F_Corridor_Text_YellowRivalVictory, NULL, FALSE, TRUE, FALSE, FALSE
    .else
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, \lose_text, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
    .endif
.endm

.include "data/maps/SSAnne_2F_Corridor_Frlg/scripts_frlg_base.inc"

SSAnne_2F_Corridor_Text_YellowRivalIntro::
    .string "{RIVAL}: Bonjour!\n"
    .string "{PLAYER}!\p"
    .string "Imagine seeing you here!\p"
    .string "{PLAYER}, were you really invited?\p"
    .string "So how's your POKéDEX coming?\p"
    .string "I already caught 40 kinds, pal!\p"
    .string "Different kinds are everywhere!\p"
    .string "Crawl around in grassy areas!$"

SSAnne_2F_Corridor_Text_YellowRivalDefeat::
    .string "Humph!\p"
    .string "At least you're raising your\n"
    .string "POKéMON!$"

SSAnne_2F_Corridor_Text_YellowRivalVictory::
    .string "{PLAYER}! What are you, seasick?\p"
    .string "You should shape up, pal!$"

SSAnne_2F_Corridor_Text_YellowRivalPostBattle::
    .string "{RIVAL}: I heard there was a CUT\n"
    .string "master on board.\p"
    .string "But, he was just a seasick, old man!\p"
    .string "But, CUT itself is really useful!\p"
    .string "You should go see him! Smell ya!$"

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
