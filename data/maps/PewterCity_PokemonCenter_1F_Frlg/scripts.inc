PewterCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_PokemonCenter_1F_OnTransition
	.byte 0

PewterCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_PEWTER_CITY
	end

PewterCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox PewterCity_PokemonCenter_1F_Text_Gentleman, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_Jigglypuff::
	lock
	playbgm MUS_JIGGLYPUFF_SONG, TRUE
	message PewterCity_PokemonCenter_1F_Text_Jigglypuff
	waitmessage
	callnative GoldenYellow_CheckPartnerPikachuFollowing
	goto_if_eq VAR_RESULT, FALSE, PewterCity_PokemonCenter_1F_EventScript_JigglypuffFinish
	applymovement OBJ_EVENT_ID_NPC_FOLLOWER, PewterCity_PokemonCenter_1F_Movement_PartnerSleepy
	waitmovement OBJ_EVENT_ID_NPC_FOLLOWER
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry

PewterCity_PokemonCenter_1F_EventScript_JigglypuffFinish::
	delay 48
	playbgm MUS_POKEMON_CENTER, TRUE
	release
	end

PewterCity_PokemonCenter_1F_Movement_PartnerSleepy:
	face_down
	delay_16
	delay_16
	delay_16
	face_player
	delay_16
	delay_16
	step_end

PewterCity_PokemonCenter_1F_Text_Gentleman:
	.string "There aren't many serious POKéMON\n"
	.string "trainers here!$"

PewterCity_PokemonCenter_1F_Text_Jigglypuff:
	.string "JIGGLYPUFF: Puu pupuu!$"
