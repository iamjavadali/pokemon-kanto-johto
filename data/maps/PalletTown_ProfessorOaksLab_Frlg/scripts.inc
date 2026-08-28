@ Golden Yellow wrapper around the accepted Oak Lab choreography.
@ Keep the proven Stage 1A scene byte-for-byte in scripts_frlg_base.inc and add
@ only Yellow's persistent rival-Eevee outcome state after the early rival battle.
.set VAR_YELLOW_RIVAL_EEVEE_STATE, 0x40FD
.set YELLOW_RIVAL_EEVEE_UNSET,     0
.set YELLOW_RIVAL_EEVEE_JOLTEON,   1
.set YELLOW_RIVAL_EEVEE_FLAREON,   2
.set YELLOW_RIVAL_EEVEE_VAPOREON,  3

.purgem trainerbattle_earlyrival
.macro trainerbattle_earlyrival trainer:req, flags:req, lose_text:req, victory_text:req
    trainerbattle TRAINER_BATTLE_EARLY_RIVAL, OBJ_ID_NONE, \trainer, NULL, \lose_text, NULL, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, \victory_text, NULL, FALSE, TRUE, FALSE, FALSE, \flags
    .if (\trainer >= TRAINER_RIVAL_OAKS_LAB_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_OAKS_LAB_CHARMANDER)
        @ Yellow: win in Oak's Lab provisionally selects Flareon; a loss selects Vaporeon.
        goto_if_eq VAR_RESULT, TRUE, .LYellowRivalOakLost\@
        setvar VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_FLAREON
        goto .LYellowRivalOakStateDone\@
.LYellowRivalOakLost\@:
        setvar VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_VAPOREON
.LYellowRivalOakStateDone\@:
    .endif
.endm

.include "data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts_frlg_base.inc"

.purgem trainerbattle_earlyrival
.macro trainerbattle_earlyrival trainer:req, flags:req, lose_text:req, victory_text:req
    trainerbattle TRAINER_BATTLE_EARLY_RIVAL, OBJ_ID_NONE, \trainer, NULL, \lose_text, NULL, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, \victory_text, NULL, FALSE, TRUE, FALSE, FALSE, \flags
.endm
