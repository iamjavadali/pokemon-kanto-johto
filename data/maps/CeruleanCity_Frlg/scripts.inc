@ Golden Yellow Cerulean rival wrapper.
@ Preserve FRLG movement/geometry but route the battle through Yellow's Eevee
@ state and suppress FireRed's Fame Checker addition to the rival scene.
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
            .ifc \text,CeruleanCity_Text_OhRightLittlePresentAsFavor
                @ FireRed-only Fame Checker dialogue: intentionally omitted.
            .else
                .ifc \text,CeruleanCity_Text_ExplainFameCheckerSmellYa
                    @ FireRed-only Fame Checker dialogue: intentionally omitted.
                .else
                    loadword 0, \text
                    callstd \type
                .endif
            .endif
        .endif
    .endif
.endm

.purgem famechecker
.macro famechecker person:req, index:req, function=SetFlavorTextFlagFromSpecialVars
    .if (\person == FAMECHECKER_BILL) && (\index == 0)
        @ FireRed-only rival reward side effect: intentionally omitted.
    .else
        setvar VAR_0x8004, \person
        setvar VAR_0x8005, \index
        special \function
    .endif
.endm

.purgem setflag
.macro setflag flag:req
    .if \flag == FLAG_GOT_FAME_CHECKER
        @ Do not mark the FireRed-only rival reward as obtained.
    .else
        .byte SCR_OP_SETFLAG
        .2byte \flag
    .endif
.endm

.purgem giveitem
.macro giveitem item:req, amount=1
    .if \item == ITEM_FAME_CHECKER
        @ Do not give the FireRed-only Fame Checker during Yellow's Cerulean scene.
    .else
        setorcopyvar VAR_0x8000, \item
        setorcopyvar VAR_0x8001, \amount
        callstd STD_OBTAIN_ITEM
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
.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    loadword 0, \text
    callstd \type
.endm

.purgem famechecker
.macro famechecker person:req, index:req, function=SetFlavorTextFlagFromSpecialVars
    setvar VAR_0x8004, \person
    setvar VAR_0x8005, \index
    special \function
.endm

.purgem setflag
.macro setflag flag:req
    .byte SCR_OP_SETFLAG
    .2byte \flag
.endm

.purgem giveitem
.macro giveitem item:req, amount=1
    setorcopyvar VAR_0x8000, \item
    setorcopyvar VAR_0x8001, \amount
    callstd STD_OBTAIN_ITEM
.endm

.include "data/scripts/yellow_rival_dispatch_end.inc"
