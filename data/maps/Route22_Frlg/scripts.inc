@ Golden Yellow Route 22 rival wrapper.
.include "data/scripts/yellow_rival_dispatch_begin.inc"

.purgem trainerbattle_earlyrival
.macro trainerbattle_earlyrival trainer:req, flags:req, lose_text:req, victory_text:req
    trainerbattle TRAINER_BATTLE_EARLY_RIVAL, OBJ_ID_NONE, \trainer, NULL, \lose_text, NULL, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, \victory_text, NULL, FALSE, TRUE, FALSE, FALSE, \flags
    .if (\trainer >= TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE) && (\trainer <= TRAINER_RIVAL_ROUTE22_EARLY_CHARMANDER)
        @ Yellow: only a win here can promote the provisional Flareon lane to Jolteon.
        @ A loss leaves Flareon/Vaporeon unchanged. UNSET is a development-save fallback.
        goto_if_eq VAR_RESULT, TRUE, .LYellowRivalRoute22StateDone\@
        goto_if_eq VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_VAPOREON, .LYellowRivalRoute22StateDone\@
        setvar VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_JOLTEON
.LYellowRivalRoute22StateDone\@:
    .endif
.endm

.include "data/maps/Route22_Frlg/scripts_frlg_base.inc"

.purgem trainerbattle_earlyrival
.macro trainerbattle_earlyrival trainer:req, flags:req, lose_text:req, victory_text:req
    trainerbattle TRAINER_BATTLE_EARLY_RIVAL, OBJ_ID_NONE, \trainer, NULL, \lose_text, NULL, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, \victory_text, NULL, FALSE, TRUE, FALSE, FALSE, \flags
.endm

.include "data/scripts/yellow_rival_dispatch_end.inc"
