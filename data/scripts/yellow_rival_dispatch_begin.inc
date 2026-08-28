@ Golden Yellow Stage 1 rival compatibility layer.
@ The retained FRLG map scripts dispatch rival teams through VAR_STARTER_MON.
@ Yellow has a fixed Pikachu starter and a branching rival Eevee instead, so while
@ assembling the preserved FRLG choreography, translate only three-argument
@ starter comparisons to the dedicated Yellow rival state.
@
@ 0x40FD is unused in both the shared Emerald variable table and the FRLG table.
VAR_YELLOW_RIVAL_EEVEE_STATE = 0x40FD
YELLOW_RIVAL_EEVEE_UNSET     = 0
YELLOW_RIVAL_EEVEE_JOLTEON   = 1
YELLOW_RIVAL_EEVEE_FLAREON   = 2
YELLOW_RIVAL_EEVEE_VAPOREON  = 3

.purgem call_if_eq
.macro call_if_eq a:req, b, c
.ifnb \c
    .if \a == VAR_STARTER_MON
        .if \b == 2
            trycompare call_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_JOLTEON, \c
            @ Development-save fallback: an unset branch uses Yellow's Jolteon lane.
            trycompare call_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_UNSET, \c
        .elseif \b == 1
            trycompare call_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_FLAREON, \c
        .elseif \b == 0
            trycompare call_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_VAPOREON, \c
        .else
            trycompare call_if, EQUAL, \a, \b, \c
        .endif
    .else
        trycompare call_if, EQUAL, \a, \b, \c
    .endif
.else
    trycompare call_if, EQUAL, \a, \b, \c
.endif
.endm

.purgem goto_if_eq
.macro goto_if_eq a:req, b, c
.ifnb \c
    .if \a == VAR_STARTER_MON
        .if \b == 2
            trycompare goto_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_JOLTEON, \c
            trycompare goto_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_UNSET, \c
        .elseif \b == 1
            trycompare goto_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_FLAREON, \c
        .elseif \b == 0
            trycompare goto_if, EQUAL, VAR_YELLOW_RIVAL_EEVEE_STATE, YELLOW_RIVAL_EEVEE_VAPOREON, \c
        .else
            trycompare goto_if, EQUAL, \a, \b, \c
        .endif
    .else
        trycompare goto_if, EQUAL, \a, \b, \c
    .endif
.else
    trycompare goto_if, EQUAL, \a, \b, \c
.endif
.endm
