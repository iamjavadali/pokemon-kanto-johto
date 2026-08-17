PewterCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PewterCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_PEWTER_CITY
	end

PewterCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

PewterCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox PewterCity_PokemonCenter_1F_Text_TeamRocketMtMoonImOnPhone, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_Jigglypuff::
	lock
	faceplayer
	fadeoutbgm 0
	playbgm MUS_RG_JIGGLYPUFF, 0
	message PewterCity_PokemonCenter_1F_Text_Jigglypuff
	waitmessage
	delay 455
	playbgm MUS_POKE_CENTER, 1
	closemessage
	delay 16
	goto_if_unset FLAG_PARTNER_PIKACHU_FOLLOWING, PewterCity_PokemonCenter_1F_EventScript_JigglypuffFinish
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_PIKACHU_STARTER, PewterCity_PokemonCenter_1F_EventScript_PartnerSleepy
	setvar VAR_0x8004, 1
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_PIKACHU_STARTER, PewterCity_PokemonCenter_1F_EventScript_PartnerSleepy
	setvar VAR_0x8004, 2
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_PIKACHU_STARTER, PewterCity_PokemonCenter_1F_EventScript_PartnerSleepy
	setvar VAR_0x8004, 3
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_PIKACHU_STARTER, PewterCity_PokemonCenter_1F_EventScript_PartnerSleepy
	setvar VAR_0x8004, 4
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_PIKACHU_STARTER, PewterCity_PokemonCenter_1F_EventScript_PartnerSleepy
	setvar VAR_0x8004, 5
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_ne VAR_RESULT, SPECIES_PIKACHU_STARTER, PewterCity_PokemonCenter_1F_EventScript_JigglypuffFinish

PewterCity_PokemonCenter_1F_EventScript_PartnerSleepy::
	applymovement OBJ_EVENT_ID_NPC_FOLLOWER, PewterCity_PokemonCenter_1F_Movement_PartnerSleepy
	waitmovement OBJ_EVENT_ID_NPC_FOLLOWER
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_WEAK
	waitmoncry
	delay 32

PewterCity_PokemonCenter_1F_EventScript_JigglypuffFinish::
	release
	end

PewterCity_PokemonCenter_1F_Movement_PartnerSleepy:
	face_down
	delay_16
	walk_in_place_slow_down
	walk_in_place_slow_down
	delay_16
	delay_16
	delay_16
	face_player
	delay_16
	step_end

PewterCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox PewterCity_PokemonCenter_1F_Text_WhenJiggylypuffSingsMonsGetDrowsy, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_GBAKid1::
	lock
	msgbox PewterCity_PokemonCenter_1F_Text_TradingMyClefairyForPikachu
	release
	end

PewterCity_PokemonCenter_1F_EventScript_GBAKid2::
	msgbox PewterCity_PokemonCenter_1F_Text_TradingPikachuWithKid, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_Text_TeamRocketMtMoonImOnPhone::
	.string "What!?\p"
	.string "TEAM ROCKET is at MT. MOON?\n"
	.string "Huh?\p"
	.string "I'm on the phone!\n"
	.string "Scram!$"

PewterCity_PokemonCenter_1F_Text_Jigglypuff::
	.string "JIGGLYPUFF: Puu pupuu!$"

PewterCity_PokemonCenter_1F_Text_WhenJiggylypuffSingsMonsGetDrowsy::
	.string "Yawn!\p"
	.string "When JIGGLYPUFF sings, POKéMON get\n"
	.string "drowsy…\p"
	.string "…Me, too…\n"
	.string "Snore…$"

PewterCity_PokemonCenter_1F_Text_TradingMyClefairyForPikachu::
	.string "I really want a PIKACHU, so I'm\n"
	.string "trading my CLEFAIRY for one.$"

PewterCity_PokemonCenter_1F_Text_TradingPikachuWithKid::
	.string "I'm trading POKéMON with that kid\n"
	.string "there.\p"
	.string "I had two PIKACHU, so I figured\n"
	.string "I might as well trade one.$"