@ Golden Yellow Pokemon Tower rival wrapper.
@ The rival still carries Eevee here, while the surrounding roster reflects the
@ future Jolteon/Flareon/Vaporeon lane. Restore Yellow's rival text exactly.
.include "data/scripts/yellow_rival_dispatch_begin.inc"

.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    .ifc \text,PokemonTower_2F_Text_RivalIntro
        loadword 0, PokemonTower_2F_Text_YellowRivalIntro
        callstd \type
    .else
        .ifc \text,PokemonTower_2F_Text_RivalPostBattle
            loadword 0, PokemonTower_2F_Text_YellowRivalPostBattle
            callstd \type
        .else
            loadword 0, \text
            callstd \type
        .endif
    .endif
.endm

.purgem trainerbattle_no_intro
.macro trainerbattle_no_intro trainer:req, lose_text:req
    .if (\trainer >= TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_POKEMON_TOWER_CHARMANDER)
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, PokemonTower_2F_Text_YellowRivalDefeat, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, PokemonTower_2F_Text_YellowRivalVictory, NULL, FALSE, TRUE, FALSE, FALSE
    .else
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, \lose_text, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
    .endif
.endm

.include "data/maps/PokemonTower_2F_Frlg/scripts_frlg_base.inc"

PokemonTower_2F_Text_YellowRivalIntro::
    .string "{RIVAL}: Hey, {PLAYER}! What brings\n"
    .string "you here? Your POKéMON don't look\n"
    .string "dead!\p"
    .string "I can at least make them faint!\n"
    .string "Let's go, pal!$"

PokemonTower_2F_Text_YellowRivalDefeat::
    .string "What?\n"
    .string "You stinker!\p"
    .string "I took it easy on you too!$"

PokemonTower_2F_Text_YellowRivalVictory::
    .string "{RIVAL}: Well, look at all your\n"
    .string "wimpy POKéMON!\p"
    .string "Toughen them up a bit more!$"

PokemonTower_2F_Text_YellowRivalPostBattle::
    .string "How's your POKéDEX coming, pal?\n"
    .string "I just caught a CUBONE!\p"
    .string "I can't find the grown-up MAROWAK\n"
    .string "yet!\p"
    .string "I doubt there are any left! Well, I\n"
    .string "better get going! I've got a lot to\n"
    .string "accomplish, pal!\p"
    .string "Smell ya later!$"

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
