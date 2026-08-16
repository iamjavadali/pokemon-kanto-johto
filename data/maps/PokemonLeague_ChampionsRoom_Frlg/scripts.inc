@ Golden Yellow Champion rival wrapper.
@ Retain FRLG room choreography/lighting while selecting Yellow's Eevee branch and
@ restoring Yellow's first-Champion dialogue.
.include "data/scripts/yellow_rival_dispatch_begin.inc"

.purgem msgbox
.macro msgbox text:req, type=MSGBOX_DEFAULT
    .ifc \text,PokemonLeague_ChampionsRoom_Text_Intro
        loadword 0, PokemonLeague_ChampionsRoom_Text_YellowIntro
        callstd \type
    .else
        .ifc \text,PokemonLeague_ChampionsRoom_Text_RematchIntro
            loadword 0, PokemonLeague_ChampionsRoom_Text_YellowIntro
            callstd \type
        .else
            .ifc \text,PokemonLeague_ChampionsRoom_Text_PostBattle
                loadword 0, PokemonLeague_ChampionsRoom_Text_YellowPostBattle
                callstd \type
            .else
                .ifc \text,PokemonLeague_ChampionsRoom_Text_OakPlayer
                    loadword 0, PokemonLeague_ChampionsRoom_Text_YellowOakPlayer
                    callstd \type
                .else
                    .ifc \text,PokemonLeague_ChampionsRoom_Text_OakCongratulations
                        loadword 0, PokemonLeague_ChampionsRoom_Text_YellowOakCongratulations
                        callstd \type
                    .else
                        .ifc \text,PokemonLeague_ChampionsRoom_Text_OakImDisappointedRival
                            loadword 0, PokemonLeague_ChampionsRoom_Text_YellowOakDisappointed
                            callstd \type
                        .else
                            .ifc \text,PokemonLeague_ChampionsRoom_Text_OakPlayerComeWithMe
                                loadword 0, PokemonLeague_ChampionsRoom_Text_YellowOakComeWithMe
                                callstd \type
                            .else
                                loadword 0, \text
                                callstd \type
                            .endif
                        .endif
                    .endif
                .endif
            .endif
        .endif
    .endif
.endm

.purgem trainerbattle_no_intro
.macro trainerbattle_no_intro trainer:req, lose_text:req
    .if ((\trainer >= TRAINER_CHAMPION_FIRST_SQUIRTLE) && (\trainer <= TRAINER_CHAMPION_FIRST_CHARMANDER)) || ((\trainer >= TRAINER_CHAMPION_REMATCH_SQUIRTLE) && (\trainer <= TRAINER_CHAMPION_REMATCH_CHARMANDER))
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, PokemonLeague_ChampionsRoom_Text_YellowDefeat, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, PokemonLeague_ChampionsRoom_Text_YellowVictory, NULL, FALSE, TRUE, FALSE, FALSE
    .else
        trainerbattle TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT, LOCALID_NONE, \trainer, NULL, \lose_text, NULL, LOCALID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
    .endif
.endm

.include "data/maps/PokemonLeague_ChampionsRoom_Frlg/scripts_frlg_base.inc"

PokemonLeague_ChampionsRoom_Text_YellowIntro::
    .string "{RIVAL}: Hey!\p"
    .string "I was looking forward to seeing\n"
    .string "you, {PLAYER}!\p"
    .string "My rival should be strong to keep\n"
    .string "me sharp!\p"
    .string "While working on POKéDEX, I looked\n"
    .string "all over for powerful POKéMON!\p"
    .string "Not only that, I assembled teams\n"
    .string "that would beat any POKéMON type!\p"
    .string "And now!\p"
    .string "I'm the POKéMON LEAGUE champion!\p"
    .string "{PLAYER}! Do you know what that\n"
    .string "means?\p"
    .string "I'll tell you!\p"
    .string "I am the most powerful trainer in\n"
    .string "the world!$"

PokemonLeague_ChampionsRoom_Text_YellowDefeat::
    .string "NO!\n"
    .string "That can't be!\l"
    .string "You beat my best!\p"
    .string "After all that work to become\n"
    .string "LEAGUE champ?\p"
    .string "My reign is over already?\n"
    .string "It's not fair!$"

PokemonLeague_ChampionsRoom_Text_YellowVictory::
    .string "Hahaha!\n"
    .string "I won, I won!\p"
    .string "I'm too good for you, {PLAYER}!\p"
    .string "You did well to even reach me,\n"
    .string "{RIVAL}, the POKéMON genius!\p"
    .string "Nice try, loser!\n"
    .string "Hahaha!$"

PokemonLeague_ChampionsRoom_Text_YellowPostBattle::
    .string "Why?\n"
    .string "Why did I lose?\p"
    .string "I never made any mistakes raising\n"
    .string "my POKéMON...\p"
    .string "Darn it! You're the new POKéMON\n"
    .string "LEAGUE champion!\p"
    .string "Although I don't like to admit it.$"

PokemonLeague_ChampionsRoom_Text_YellowOakPlayer::
    .string "OAK: {PLAYER}!$"

PokemonLeague_ChampionsRoom_Text_YellowOakCongratulations::
    .string "OAK: So, you won!\n"
    .string "Congratulations!\p"
    .string "You're the new POKéMON LEAGUE\n"
    .string "champion!\p"
    .string "You've grown up so much since you\n"
    .string "first left with {STR_VAR_1}!\p"
    .string "{PLAYER}, you have come of age!$"

PokemonLeague_ChampionsRoom_Text_YellowOakDisappointed::
    .string "OAK: {RIVAL}! I'm disappointed!\p"
    .string "I came when I heard you beat the\n"
    .string "ELITE FOUR!\p"
    .string "But, when I got here, you had\n"
    .string "already lost!\p"
    .string "{RIVAL}! Do you understand why\n"
    .string "you lost?\p"
    .string "You have forgotten to treat your\n"
    .string "POKéMON with trust and love!\p"
    .string "Without them, you will never become\n"
    .string "a champ again!$"

PokemonLeague_ChampionsRoom_Text_YellowOakComeWithMe::
    .string "OAK: {PLAYER}!\p"
    .string "You understand that your victory\n"
    .string "was not just your own doing!\p"
    .string "The bond you share with your POKéMON\n"
    .string "is marvelous!\p"
    .string "{PLAYER}!\n"
    .string "Come with me!$"

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
