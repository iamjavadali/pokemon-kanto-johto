PewterCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PewterCity_PokemonCenter_1F_OnFrame
	.byte 0

PewterCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_PEWTER_CITY
	callnative GoldenYellow_RestorePewterPartnerSleepNative
	end

PewterCity_PokemonCenter_1F_OnFrame:
	map_script_2 VAR_UNUSED_0x40FB, 2, PewterCity_PokemonCenter_1F_EventScript_PartnerWakePending
	.2byte 0

PewterCity_PokemonCenter_1F_EventScript_PartnerWakePending::
	lockall
	callnative GoldenYellow_ResumePendingPewterPartnerWake
	releaseall
	end

PewterCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	goto_if_ne VAR_UNUSED_0x40FB, 0, PewterCity_PokemonCenter_1F_EventScript_NursePartnerSleeping
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

PewterCity_PokemonCenter_1F_EventScript_NursePartnerSleeping::
	msgbox PewterCity_PokemonCenter_1F_Text_PikachuContentAsleep, MSGBOX_DEFAULT
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
	applymovement LOCALID_PEWTER_JIGGLYPUFF, PewterCity_PokemonCenter_1F_Movement_JigglypuffSing
	waitmovement LOCALID_PEWTER_JIGGLYPUFF
	delay 48
	playbgm MUS_POKE_CENTER, 1
	closemessage
	delay 16
	callnative GoldenYellow_TryStartPewterPartnerSleepOnFollower
	goto_if_eq VAR_RESULT, FALSE, PewterCity_PokemonCenter_1F_EventScript_JigglypuffFinish
	applymovement OBJ_EVENT_ID_FOLLOWER, PewterCity_PokemonCenter_1F_Movement_PartnerSleepy
	waitmovement OBJ_EVENT_ID_FOLLOWER
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_WEAK
	waitmoncry
	callnative GoldenYellow_ShowFollowerSleepEmote, waitstate=1
	callnative GoldenYellow_FacePlayerTowardFollower
	delay 16

PewterCity_PokemonCenter_1F_EventScript_JigglypuffFinish::
	release
	end

PewterCity_PokemonCenter_1F_EventScript_BlockFrontExit::
	lockall
	applymovement LOCALID_PEWTER_NURSE, PewterCity_PokemonCenter_1F_Movement_NurseStopPlayer
	waitmovement LOCALID_PEWTER_NURSE
	msgbox PewterCity_PokemonCenter_1F_Text_NurseDontLeaveSleepingPikachu, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, PewterCity_PokemonCenter_1F_Movement_PlayerBackFromFrontExit
	waitmovement 0
	releaseall
	end

PewterCity_PokemonCenter_1F_EventScript_BlockUpstairs::
	lockall
	applymovement LOCALID_PEWTER_NURSE, PewterCity_PokemonCenter_1F_Movement_NurseStopPlayer
	waitmovement LOCALID_PEWTER_NURSE
	msgbox PewterCity_PokemonCenter_1F_Text_NurseDontLeaveSleepingPikachu, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, PewterCity_PokemonCenter_1F_Movement_PlayerBackFromUpstairs
	waitmovement 0
	releaseall
	end

PewterCity_PokemonCenter_1F_Movement_JigglypuffSing:
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	face_left
	delay_16
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	step_end

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

PewterCity_PokemonCenter_1F_Movement_NurseStopPlayer:
	emote_exclamation_mark
	face_down
	step_end

PewterCity_PokemonCenter_1F_Movement_PlayerBackFromFrontExit:
	walk_up
	step_end

PewterCity_PokemonCenter_1F_Movement_PlayerBackFromUpstairs:
	walk_right
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

PewterCity_PokemonCenter_1F_Text_PikachuContentAsleep::
	.string "It looks very content asleep.$"

PewterCity_PokemonCenter_1F_Text_NurseDontLeaveSleepingPikachu::
	.string "Wait! Your PIKACHU is still fast\n"
	.string "asleep.\p"
	.string "You shouldn't leave it behind.$"

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
