MtMoon_B2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_B2F_OnTransition
	.byte 0

MtMoon_B2F_OnTransition::
	call_if_unset FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_ShowFossils
	goto_if_defeated TRAINER_JESSIE_JAMES_MT_MOON, MtMoon_B2F_EventScript_JessieJamesCompletedTransition
	goto_if_eq VAR_MAP_SCENE_MT_MOON_B2F, 2, MtMoon_B2F_EventScript_ShowJessieJames
	clearflag FLAG_0x0B9
	setflag FLAG_0x0BA
	removeobject LOCALID_THEFT_MEOWTH
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	setflag FLAG_0x0B7
	setflag FLAG_0x0B8
	removeobject LOCALID_JESSIE
	removeobject LOCALID_JAMES
	removeobject LOCALID_ROCKET_MEOWTH
	removeobject LOCALID_DROPPED_FOSSIL
	end

MtMoon_B2F_EventScript_ShowJessieJames::
	setflag FLAG_0x0BA
	removeobject LOCALID_THEFT_MEOWTH
	setflag FLAG_0x0B9
	removeobject LOCALID_MIGUEL
	setflag FLAG_0x0B8
	removeobject LOCALID_DROPPED_FOSSIL
	clearflag FLAG_0x0B5
	clearflag FLAG_0x0B6
	clearflag FLAG_0x0B7
	addobject LOCALID_JESSIE
	addobject LOCALID_ROCKET_MEOWTH
	addobject LOCALID_JAMES
	end

MtMoon_B2F_EventScript_JessieJamesCompletedTransition::
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 3
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	setflag FLAG_0x0B7
	removeobject LOCALID_JESSIE
	removeobject LOCALID_JAMES
	removeobject LOCALID_ROCKET_MEOWTH
	setflag FLAG_0x0BA
	removeobject LOCALID_THEFT_MEOWTH
	clearflag FLAG_0x0B9
	addobject LOCALID_MIGUEL
	setobjectxy LOCALID_MIGUEL, 13, 11
	goto_if_set FLAG_GOT_DOME_FOSSIL, MtMoon_B2F_EventScript_CheckRecoveredHelix
	goto MtMoon_B2F_EventScript_ShowDroppedFossilAfterRocket

MtMoon_B2F_EventScript_CheckRecoveredHelix::
	goto_if_set FLAG_GOT_HELIX_FOSSIL, MtMoon_B2F_EventScript_HideDroppedFossilAfterPickup
	goto MtMoon_B2F_EventScript_ShowDroppedFossilAfterRocket

MtMoon_B2F_EventScript_ShowDroppedFossilAfterRocket::
	clearflag FLAG_0x0B8
	addobject LOCALID_DROPPED_FOSSIL
	end

MtMoon_B2F_EventScript_HideDroppedFossilAfterPickup::
	setflag FLAG_0x0B8
	removeobject LOCALID_DROPPED_FOSSIL
	end

MtMoon_B2F_EventScript_ShowFossils::
	clearflag FLAG_HIDE_DOME_FOSSIL
	clearflag FLAG_HIDE_HELIX_FOSSIL
	return

MtMoon_B2F_EventScript_MiguelTrigger::
	lockall
	applymovement LOCALID_MIGUEL, Common_Movement_WalkInPlaceFasterRight
	waitmovement LOCALID_MIGUEL
	call MtMoon_B2F_EventScript_BattleMiguel
	releaseall
	end

MtMoon_B2F_EventScript_Miguel::
	lock
	faceplayer
	goto_if_set FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_MiguelFossilPicked
	goto_if_defeated TRAINER_SUPER_NERD_MIGUEL, MtMoon_B2F_EventScript_MiguelGoPickFossil
	call MtMoon_B2F_EventScript_BattleMiguel
	release
	end

MtMoon_B2F_EventScript_BattleMiguel::
	textcolor NPC_TEXT_COLOR_MALE
	message MtMoon_B2F_Text_MiguelIntro
	waitmessage
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setvar VAR_LAST_TALKED, LOCALID_MIGUEL
	trainerbattle_no_intro TRAINER_SUPER_NERD_MIGUEL, MtMoon_B2F_Text_MiguelDefeat
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 1
	msgbox MtMoon_B2F_Text_WellEachTakeAFossil
	return

MtMoon_B2F_EventScript_MiguelFossilPicked::
	msgbox MtMoon_B2F_Text_LabOnCinnabarRegeneratesFossils
	release
	end

MtMoon_B2F_EventScript_MiguelGoPickFossil::
	msgbox MtMoon_B2F_Text_WellEachTakeAFossil
	release
	end

MtMoon_B2F_EventScript_DomeFossil::
	lockall
	faceplayer
	msgbox MtMoon_B2F_Text_YouWantDomeFossil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtMoon_B2F_EventScript_DontTakeFossilAll
	removeobject LOCALID_DOME_FOSSIL
	setflag FLAG_HIDE_DOME_FOSSIL
	giveitem_msg MtMoon_B2F_Text_ObtainedDomeFossil, ITEM_DOME_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	closemessage
	delay 10
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToHelixFossil
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	waitmovement LOCALID_MIGUEL
	textcolor NPC_TEXT_COLOR_MALE
	msgbox MtMoon_B2F_Text_ThenThisFossilIsMine
	setflag FLAG_GOT_DOME_FOSSIL
	setflag FLAG_GOT_FOSSIL_FROM_MT_MOON
	call MtMoon_B2F_EventScript_MeowthStealsHelix
	releaseall
	end

MtMoon_B2F_EventScript_HelixFossil::
	lockall
	faceplayer
	msgbox MtMoon_B2F_Text_YouWantHelixFossil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtMoon_B2F_EventScript_DontTakeFossilAll
	removeobject LOCALID_HELIX_FOSSIL
	setflag FLAG_HIDE_HELIX_FOSSIL
	giveitem_msg MtMoon_B2F_Text_ObtainedHelixFossil, ITEM_HELIX_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	closemessage
	delay 10
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToDomeFossil
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	waitmovement LOCALID_MIGUEL
	textcolor NPC_TEXT_COLOR_MALE
	msgbox MtMoon_B2F_Text_ThenThisFossilIsMine
	setflag FLAG_GOT_HELIX_FOSSIL
	setflag FLAG_GOT_FOSSIL_FROM_MT_MOON
	call MtMoon_B2F_EventScript_MeowthStealsDome
	releaseall
	end

MtMoon_B2F_EventScript_DontTakeFossilAll::
	releaseall
	end

MtMoon_B2F_EventScript_MeowthStealsHelix::
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	setflag FLAG_0x0B8
	clearflag FLAG_0x0BA
	addobject LOCALID_THEFT_MEOWTH
	fadenewbgm MUS_RG_ROCKET_HIDEOUT
	delay 1
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthUpperCorridorToHelix
	waitmovement LOCALID_THEFT_MEOWTH
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthStepOntoFossil
	waitmovement LOCALID_THEFT_MEOWTH
	removeobject LOCALID_HELIX_FOSSIL
	setflag FLAG_HIDE_HELIX_FOSSIL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MeowthStoleFossil
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelPanic
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MiguelAfterTheFossil
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthEscapeHelixFull
	playse SE_PIN
	applymovement LOCALID_MIGUEL, Common_Movement_ExclamationMark
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, Common_Movement_Delay48
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelChaseHelix
	waitmovement LOCALID_THEFT_MEOWTH
	setflag FLAG_0x0BA
	removeobject LOCALID_THEFT_MEOWTH
	waitmovement LOCALID_MIGUEL
	setflag FLAG_0x0B9
	removeobject LOCALID_MIGUEL
	goto MtMoon_B2F_EventScript_StageJessieJames

MtMoon_B2F_EventScript_MeowthStealsDome::
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	setflag FLAG_0x0B8
	clearflag FLAG_0x0BA
	addobject LOCALID_THEFT_MEOWTH
	fadenewbgm MUS_RG_ROCKET_HIDEOUT
	delay 1
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthUpperCorridorToDome
	waitmovement LOCALID_THEFT_MEOWTH
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthStepOntoFossil
	waitmovement LOCALID_THEFT_MEOWTH
	removeobject LOCALID_DOME_FOSSIL
	setflag FLAG_HIDE_DOME_FOSSIL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MeowthStoleFossil
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelPanic
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MiguelAfterTheFossil
	applymovement LOCALID_THEFT_MEOWTH, MtMoon_B2F_Movement_MeowthEscapeDomeFull
	playse SE_PIN
	applymovement LOCALID_MIGUEL, Common_Movement_ExclamationMark
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, Common_Movement_Delay48
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelChaseDome
	waitmovement LOCALID_THEFT_MEOWTH
	setflag FLAG_0x0BA
	removeobject LOCALID_THEFT_MEOWTH
	waitmovement LOCALID_MIGUEL
	setflag FLAG_0x0B9
	removeobject LOCALID_MIGUEL
	goto MtMoon_B2F_EventScript_StageJessieJames

MtMoon_B2F_EventScript_StageJessieJames::
	setflag FLAG_0x0B9
	removeobject LOCALID_MIGUEL
	setflag FLAG_0x0BA
	removeobject LOCALID_THEFT_MEOWTH
	clearflag FLAG_0x0B5
	clearflag FLAG_0x0B6
	clearflag FLAG_0x0B7
	addobject LOCALID_JESSIE
	addobject LOCALID_ROCKET_MEOWTH
	addobject LOCALID_JAMES
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 2
	return

MtMoon_B2F_Movement_MiguelToHelixFossil::
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

MtMoon_B2F_Movement_MiguelToDomeFossil::
	walk_up
	walk_up
	walk_up
	step_end

MtMoon_B2F_Movement_MiguelFaceDown::
	face_down
	step_end

MtMoon_B2F_Movement_MiguelFaceUp::
	face_up
	step_end

MtMoon_B2F_Movement_MeowthFaceDown::
	face_down
	step_end

MtMoon_B2F_Movement_MeowthUpperCorridorToHelix::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_down
	step_end

MtMoon_B2F_Movement_MeowthUpperCorridorToDome::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_down
	step_end

MtMoon_B2F_Movement_MeowthStepOntoFossil::
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_MeowthEscapeHelixFull::
	walk_fast_up
	walk_fast_up
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

MtMoon_B2F_Movement_MeowthEscapeDomeFull::
	walk_fast_up
	walk_fast_up
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

MtMoon_B2F_Movement_MiguelPanic::
	walk_in_place_faster_down
	walk_in_place_faster_up
	walk_in_place_faster_down
	step_end

MtMoon_B2F_Movement_MiguelChaseHelix::
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

MtMoon_B2F_Movement_MiguelChaseDome::
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

MtMoon_B2F_EventScript_JessieJamesActor::
	lockall
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTrigger::
	lockall
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX8Y3::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX8Y3
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX8Y3
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX7Y3::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX7Y3
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX7Y3
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX8Y4::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX8Y4
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX8Y4
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX7Y4::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX7Y4
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX7Y4
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX8Y5::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX8Y5
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX8Y5
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX7Y5::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX7Y5
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX7Y5
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX8Y6::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX8Y6
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX8Y6
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesTriggerX7Y6::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 56
	fadenewbgm MUS_RG_ENCOUNTER_ROCKET
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerApproachRocketX7Y6
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerApproachRocketX7Y6
	waitmovement 0
	goto MtMoon_B2F_EventScript_JessieJamesFaceoff

MtMoon_B2F_EventScript_JessieJamesFaceoff::
	setobjectxy LOCALID_JESSIE, 2, 8
	setobjectxy LOCALID_ROCKET_MEOWTH, 3, 8
	setobjectxy LOCALID_JAMES, 4, 8
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerFaceTeamRocket
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerFaceTeamRocket
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketFacePlayer
	waitmovement 0
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JESSIE
	msgbox MtMoon_B2F_Text_JessieStopRightThere
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JAMES
	msgbox MtMoon_B2F_Text_JamesFossilIsOurs
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JESSIE
	msgbox MtMoon_B2F_Text_JessieBossWantsTreasure
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JAMES
	msgbox MtMoon_B2F_Text_JamesNoEmptyHands
	playse SE_PIN
	applymovement LOCALID_JESSIE, Common_Movement_ExclamationMark
	applymovement LOCALID_JAMES, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_JESSIE, Common_Movement_Delay48
	applymovement LOCALID_JAMES, Common_Movement_Delay48
	waitmovement 0
	msgbox MtMoon_B2F_Text_JessieNoticePikachu
	msgbox MtMoon_B2F_Text_JamesPikachuUnusual
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerChallenge
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_ROCKET_MEOWTH
	msgbox MtMoon_B2F_Text_MeowthUnderstandsPikachu
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JESSIE
	msgbox MtMoon_B2F_Text_JessieRealPrize
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JAMES
	msgbox MtMoon_B2F_Text_JamesTakeBoth
	setvar VAR_LAST_TALKED, LOCALID_JESSIE
	trainerbattle_no_intro TRAINER_JESSIE_JAMES_MT_MOON, MtMoon_B2F_Text_JessieJamesDefeat
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 3
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 3, 7
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerFaceTeamRocket
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerFaceTeamRocket
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketFacePlayer
	waitmovement 0
	clearflag FLAG_0x0B8
	addobject LOCALID_DROPPED_FOSSIL
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JESSIE
	msgbox MtMoon_B2F_Text_JessieDroppedFossil
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JAMES
	msgbox MtMoon_B2F_Text_JamesBossWantedTreasure
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JESSIE
	msgbox MtMoon_B2F_Text_JessieCantReturn
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_ROCKET_MEOWTH
	msgbox MtMoon_B2F_Text_MeowthPikachuPrize
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JAMES
	msgbox MtMoon_B2F_Text_JamesPikachuSpirit
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketSpeak
	waitmovement LOCALID_JESSIE
	msgbox MtMoon_B2F_Text_JessieNextTime
	msgbox MtMoon_B2F_Text_JessieJamesBlastOff
	closemessage
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_JessieBlastOff
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketMeowthBlastOff
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_JamesBlastOff
	waitmovement 0
	removeobject LOCALID_ROCKET_MEOWTH
	removeobject LOCALID_JESSIE
	removeobject LOCALID_JAMES
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	setflag FLAG_0x0B7
	delay 32
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerClearMiguelRoute
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerClearMiguelRoute
	waitmovement 0
	clearflag FLAG_0x0B9
	addobject LOCALID_MIGUEL
	setobjectxy LOCALID_MIGUEL, 5, 10
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFromStairsToDroppedFossil
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerFaceMiguel
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerFaceMiguel
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceDroppedFossil
	waitmovement 0
	msgbox MtMoon_B2F_Text_MiguelYouRecoveredIt
	msgbox MtMoon_B2F_Text_MiguelKeepRecoveredFossil
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelExitViaMeowthLane
	waitmovement LOCALID_MIGUEL
	setflag FLAG_0x0B9
	removeobject LOCALID_MIGUEL
	setobjectxy LOCALID_MIGUEL, 13, 11
	clearflag FLAG_0x0B9
	addobject LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceRight
	waitmovement LOCALID_MIGUEL
	copyobjectxytoperm LOCALID_MIGUEL
	fadedefaultbgm
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

MtMoon_B2F_Movement_PlayerApproachRocketX8Y3::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PlayerApproachRocketX7Y3::
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PlayerApproachRocketX8Y4::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PlayerApproachRocketX7Y4::
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PlayerApproachRocketX8Y5::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PlayerApproachRocketX7Y5::
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PlayerApproachRocketX8Y6::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PlayerApproachRocketX7Y6::
	walk_left
	walk_left
	walk_left
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX8Y3::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX7Y3::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX8Y4::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX7Y4::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX8Y5::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX7Y5::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX8Y6::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerApproachRocketX7Y6::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	face_down
	step_end

MtMoon_B2F_EventScript_DroppedFossil::
	lockall
	msgbox MtMoon_B2F_Text_DroppedFossil, MSGBOX_SIGN
	goto_if_set FLAG_GOT_DOME_FOSSIL, MtMoon_B2F_EventScript_PickupRecoveredHelixFossil
	giveitem_msg MtMoon_B2F_Text_ObtainedDomeFossil, ITEM_DOME_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	setflag FLAG_GOT_DOME_FOSSIL
	goto MtMoon_B2F_EventScript_FinishDroppedFossilPickup

MtMoon_B2F_EventScript_PickupRecoveredHelixFossil::
	giveitem_msg MtMoon_B2F_Text_ObtainedHelixFossil, ITEM_HELIX_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	setflag FLAG_GOT_HELIX_FOSSIL

MtMoon_B2F_EventScript_FinishDroppedFossilPickup::
	closemessage
	removeobject LOCALID_DROPPED_FOSSIL
	setflag FLAG_0x0B8
	releaseall
	end

MtMoon_B2F_Movement_PlayerFaceTeamRocket::
	face_down
	step_end

MtMoon_B2F_Movement_PartnerFaceTeamRocket::
	face_down
	step_end

MtMoon_B2F_Movement_PartnerChallenge::
	jump_in_place_down
	step_end

MtMoon_B2F_Movement_RocketFacePlayer::
	face_up
	step_end

MtMoon_B2F_Movement_RocketFaceUp::
	face_up
	step_end

MtMoon_B2F_Movement_RocketSpeak::
	walk_in_place_faster_up
	step_end

MtMoon_B2F_Movement_JessieBlastOff::
	spin_up
	spin_up
	spin_up
	spin_up
	spin_up
	set_invisible
	step_end

MtMoon_B2F_Movement_RocketMeowthBlastOff::
	delay_4
	spin_up
	spin_up
	spin_up
	spin_up
	spin_up
	set_invisible
	step_end

MtMoon_B2F_Movement_JamesBlastOff::
	delay_8
	spin_up
	spin_up
	spin_up
	spin_up
	spin_up
	set_invisible
	step_end

MtMoon_B2F_Movement_PlayerClearMiguelRoute::
	walk_left
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_PartnerClearMiguelRoute::
	walk_left
	walk_down
	face_down
	step_end

MtMoon_B2F_Movement_MiguelFromStairsToDroppedFossil::
	walk_up
	walk_left
	face_left
	step_end

MtMoon_B2F_Movement_MiguelFaceDroppedFossil::
	face_left
	step_end

MtMoon_B2F_Movement_MiguelToDroppedFossil::
	walk_left
	walk_left
	walk_left
	face_left
	step_end

MtMoon_B2F_Movement_MiguelReturnFromDroppedFossil::
	walk_right
	walk_right
	walk_right
	face_right
	step_end

MtMoon_B2F_Movement_MiguelExitViaMeowthLane::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_right
	step_end

MtMoon_B2F_Movement_PlayerFaceMiguel::
	face_down
	step_end

MtMoon_B2F_Movement_PartnerFaceMiguel::
	face_down
	step_end

MtMoon_B2F_Movement_MiguelFacePlayer::
	face_up
	step_end

MtMoon_B2F_Movement_MiguelFaceRight::
	face_right
	step_end

MtMoon_B2F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT, MtMoon_B2F_Text_Grunt1Intro, MtMoon_B2F_Text_Grunt1Defeat
	msgbox MtMoon_B2F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_2, MtMoon_B2F_Text_Grunt2Intro, MtMoon_B2F_Text_Grunt2Defeat
	msgbox MtMoon_B2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_3, MtMoon_B2F_Text_Grunt3Intro, MtMoon_B2F_Text_Grunt3Defeat
	msgbox MtMoon_B2F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt4::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_4, MtMoon_B2F_Text_Grunt4Intro, MtMoon_B2F_Text_Grunt4Defeat
	msgbox MtMoon_B2F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_Text_JessieStopRightThere::
	.string "JESSIE: Stop right there!$"

MtMoon_B2F_Text_JamesFossilIsOurs::
	.string "JAMES: That fossil belongs\n"
	.string "to TEAM ROCKET now!$"

MtMoon_B2F_Text_JessieBossWantsTreasure::
	.string "JESSIE: The BOSS wants\n"
	.string "rare treasure!$"

MtMoon_B2F_Text_JamesNoEmptyHands::
	.string "JAMES: We do not return\n"
	.string "empty-handed!$"

MtMoon_B2F_Text_JessieNoticePikachu::
	.string "JESSIE: Wait...\p"
	.string "Why is that PIKACHU\n"
	.string "walking loose?$"

MtMoon_B2F_Text_JamesPikachuUnusual::
	.string "JAMES: It acts too proud\n"
	.string "for its POKé BALL.$"

MtMoon_B2F_Text_MeowthUnderstandsPikachu::
	.string "MEOWTH: That PIKACHU\n"
	.string "has a rare spark!\p"
	.string "It chooses who to follow!$"

MtMoon_B2F_Text_JessieRealPrize::
	.string "JESSIE: Then PIKACHU\n"
	.string "is the real prize!$"

MtMoon_B2F_Text_JamesTakeBoth::
	.string "JAMES: We will take the\n"
	.string "fossil and PIKACHU!$"

MtMoon_B2F_Text_JessieJamesDefeat::
	.string "A brat beat us?$"

MtMoon_B2F_Text_JessieDroppedFossil::
	.string "JESSIE: The fossil!\n"
	.string "We dropped it!$"

MtMoon_B2F_Text_JamesBossWantedTreasure::
	.string "JAMES: The BOSS wanted\n"
	.string "rare treasure!$"

MtMoon_B2F_Text_JessieCantReturn::
	.string "JESSIE: We cannot go\n"
	.string "back with nothing!$"

MtMoon_B2F_Text_MeowthPikachuPrize::
	.string "MEOWTH: Forget the rock!\n"
	.string "That PIKACHU is the prize!$"

MtMoon_B2F_Text_JamesPikachuSpirit::
	.string "JAMES: A PIKACHU with\n"
	.string "that spirit is rare!$"

MtMoon_B2F_Text_JessieNextTime::
	.string "JESSIE: Next time,\n"
	.string "PIKACHU is ours!$"

MtMoon_B2F_Text_JessieJamesBlastOff::
	.string "TEAM ROCKET: Blast off\n"
	.string "at the speed of light!$"

MtMoon_B2F_Text_MiguelIntro::
	.string "Hey, stop!\p"
	.string "I found these fossils!\n"
	.string "They're both mine!$"

MtMoon_B2F_Text_MiguelDefeat::
	.string "Okay!\n"
	.string "I'll share!$"

MtMoon_B2F_Text_WellEachTakeAFossil::
	.string "We'll each take a fossil!\n"
	.string "No being greedy!$"

MtMoon_B2F_Text_ThenThisFossilIsMine::
	.string "All right.\n"
	.string "Then this fossil is mine!$"

MtMoon_B2F_Text_MeowthStoleFossil::
	.string "MEOWTH snatched the other fossil!$"

MtMoon_B2F_Text_MiguelAfterTheFossil::
	.string "Hey! My fossil!\n"
	.string "Come back here!$"

MtMoon_B2F_Text_MiguelYouRecoveredIt::
	.string "You got it back!\n"
	.string "That was my fossil...$"

MtMoon_B2F_Text_MiguelKeepRecoveredFossil::
	.string "You beat TEAM ROCKET for it.\p"
	.string "Go ahead. Keep that fossil, too.\p"
	.string "The lab on CINNABAR ISLAND can\n"
	.string "regenerate fossils into POKéMON.$"

MtMoon_B2F_Text_DroppedFossil::
	.string "TEAM ROCKET dropped the stolen\n"
	.string "fossil.$"

MtMoon_B2F_Text_LabOnCinnabarRegeneratesFossils::
	.string "Far away, on CINNABAR ISLAND,\n"
	.string "there's a POKéMON LAB.\p"
	.string "They do research on regenerating\n"
	.string "fossils.$"

MtMoon_B2F_Text_Grunt1Intro::
	.string "We, TEAM ROCKET, shall find the\n"
	.string "fossils!\p"
	.string "Reviving POKéMON from them will\n"
	.string "earn us huge riches!$"

MtMoon_B2F_Text_Grunt1Defeat::
	.string "Urgh!\n"
	.string "Now I'm mad!$"

MtMoon_B2F_Text_Grunt1PostBattle::
	.string "You made me mad!\n"
	.string "TEAM ROCKET will blacklist you!$"

MtMoon_B2F_Text_Grunt2Intro::
	.string "We, TEAM ROCKET, are POKéMON\n"
	.string "gangsters!\l"
	.string "We strike fear with our strength!$"

MtMoon_B2F_Text_Grunt2Defeat::
	.string "I blew it!$"

MtMoon_B2F_Text_Grunt2PostBattle::
	.string "Darn it all!\n"
	.string "My associates won't stand for this!$"

MtMoon_B2F_Text_Grunt3Intro::
	.string "We're pulling a big job here!\n"
	.string "Get lost, kid!$"

MtMoon_B2F_Text_Grunt3Defeat::
	.string "So, you are good…$"

MtMoon_B2F_Text_Grunt3PostBattle::
	.string "If you find a fossil, give it to me\n"
	.string "and scram!$"

MtMoon_B2F_Text_Grunt4Intro::
	.string "Little kids shouldn't be messing\n"
	.string "around with grown-ups!\p"
	.string "It could be bad news!$"

MtMoon_B2F_Text_Grunt4Defeat::
	.string "I'm steamed!$"

MtMoon_B2F_Text_Grunt4PostBattle::
	.string "POKéMON lived here long before\n"
	.string "people came.$"

MtMoon_B2F_Text_YouWantDomeFossil::
	.string "Do you want the DOME FOSSIL?$"

MtMoon_B2F_Text_YouWantHelixFossil::
	.string "Do you want the HELIX FOSSIL?$"

MtMoon_B2F_Text_ObtainedHelixFossil::
	.string "Obtained the HELIX FOSSIL!$"

MtMoon_B2F_Text_ObtainedDomeFossil::
	.string "Obtained the DOME FOSSIL!$"
