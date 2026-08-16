@ Golden Yellow Silph Co. rival wrapper.
@ Preserve the FRLG 7F movement scene, select the evolved Eevee team from Yellow's
@ persistent rival state, and restore Yellow's rival wording.
.include "data/scripts/yellow_rival_dispatch_begin.inc"

.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    .ifc \text,SilphCo_7F_Text_RivalWhatKeptYou
        loadword 0, SilphCo_7F_Text_YellowRivalWhatKeptYou
        callstd \type
    .else
        .ifc \text,SilphCo_7F_Text_RivalIntro
            loadword 0, SilphCo_7F_Text_YellowRivalIntro
            callstd \type
        .else
            .ifc \text,SilphCo_7F_Text_RivalPostBattle
                loadword 0, SilphCo_7F_Text_YellowRivalPostBattle
                callstd \type
            .else
                loadword 0, \text
                callstd \type
            .endif
        .endif
    .endif
.endm

.purgem trainerbattle_no_intro
.macro trainerbattle_no_intro trainer:req, lose_text:req
    .if (\trainer >= TRAINER_RIVAL_SILPH_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_SILPH_CHARMANDER)
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, SilphCo_7F_Text_YellowRivalDefeat, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, SilphCo_7F_Text_YellowRivalVictory, NULL, FALSE, TRUE, FALSE, FALSE
    .else
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, \lose_text, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
    .endif
.endm

.include "data/maps/SilphCo_7F_Frlg/scripts_frlg_base.inc"

SilphCo_7F_Text_YellowRivalWhatKeptYou::
    .string "{RIVAL}: What kept you {PLAYER}?$"

SilphCo_7F_Text_YellowRivalIntro::
    .string "{RIVAL}: Hahaha! I thought you'd\n"
    .string "turn up if I waited here!\p"
    .string "I guess TEAM ROCKET slowed you\n"
    .string "down! Not that I care!\p"
    .string "I saw you in SAFFRON, so I decided\n"
    .string "to see if you got better!$"

SilphCo_7F_Text_YellowRivalDefeat::
    .string "Oh-oh!\n"
    .string "So, you are ready for BOSS ROCKET!$"

SilphCo_7F_Text_YellowRivalVictory::
    .string "{RIVAL}: How can I put this?\p"
    .string "You're not good enough to play with\n"
    .string "us big boys!$"

SilphCo_7F_Text_YellowRivalPostBattle::
    .string "Well, {PLAYER}!\p"
    .string "I'm moving on up and ahead!\p"
    .string "By checking my POKéDEX, I'm\n"
    .string "starting to see what's strong and\n"
    .string "how they evolve!\p"
    .string "I'm going to the POKéMON LEAGUE to\n"
    .string "boot out the ELITE FOUR!\p"
    .string "I'll become the world's most\n"
    .string "powerful TRAINER!\p"
    .string "{PLAYER}, well good luck to you!\n"
    .string "Don't sweat it! Smell ya!$"

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
