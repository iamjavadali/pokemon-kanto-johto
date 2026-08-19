MtMoon_B2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_B2F_OnTransition
	.byte 0

MtMoon_B2F_OnTransition::
	call_if_unset FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_ShowFossils
	goto_if_defeated TRAINER_JESSIE_JAMES_MT_MOON, MtMoon_B2F_EventScript_JessieJamesCompletedTransition
	goto_if_eq VAR_MAP_SCENE_MT_MOON_B2F, 2, MtMoon_B2F_EventScript_ShowJessieJames
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
	setflag FLAG_0x0B8
	removeobject LOCALID_DROPPED_FOSSIL
	removeobject LOCALID_MIGUEL
	clearflag FLAG_0x0B5
	clearflag FLAG_0x0B6
	clearflag FLAG_0x0B7
	setobjectxy LOCALID_JESSIE, 5, 11
	setobjectxy LOCALID_ROCKET_MEOWTH, 6, 11
	setobjectxy LOCALID_JAMES, 7, 11
	addobject LOCALID_JESSIE
	addobject LOCALID_ROCKET_MEOWTH
	addobject LOCALID_JAMES
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketFaceUp
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketFaceUp
	waitmovement 0
	end

MtMoon_B2F_EventScript_JessieJamesCompletedTransition::
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 3
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	setflag FLAG_0x0B7
	setflag FLAG_0x0B8
	removeobject LOCALID_JESSIE
	removeobject LOCALID_JAMES
	removeobject LOCALID_ROCKET_MEOWTH
	removeobject LOCALID_DROPPED_FOSSIL
	setobjectxy LOCALID_MIGUEL, 13, 11
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
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceDown
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
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceDown
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

@ Meowth uses the walkable upper corridor (y=6), drops through the fossil
@ opening, then reverses that same corridor while Miguel follows one tile back.
MtMoon_B2F_EventScript_MeowthStealsHelix::
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	clearflag FLAG_0x0B7
	setflag FLAG_0x0B8
	setobjectxy LOCALID_ROCKET_MEOWTH, 6, 6
	addobject LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthUpperCorridorToHelix
	waitmovement LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	waitmoncry
	removeobject LOCALID_HELIX_FOSSIL
	setflag FLAG_HIDE_HELIX_FOSSIL
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthStepOntoFossil
	waitmovement LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MeowthStoleFossil
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelPanic
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MiguelAfterTheFossil
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthEscapeHelixHeadStart
	waitmovement LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthEscapeHelixCorridor
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelChaseHelix
	waitmovement 0
	goto MtMoon_B2F_EventScript_StageJessieJames

MtMoon_B2F_EventScript_MeowthStealsDome::
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	clearflag FLAG_0x0B7
	setflag FLAG_0x0B8
	setobjectxy LOCALID_ROCKET_MEOWTH, 6, 6
	addobject LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthUpperCorridorToDome
	waitmovement LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	waitmoncry
	removeobject LOCALID_DOME_FOSSIL
	setflag FLAG_HIDE_DOME_FOSSIL
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthStepOntoFossil
	waitmovement LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MeowthStoleFossil
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelPanic
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthFaceDown
	waitmovement 0
	msgbox MtMoon_B2F_Text_MiguelAfterTheFossil
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthEscapeDomeHeadStart
	waitmovement LOCALID_ROCKET_MEOWTH
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_MeowthEscapeDomeCorridor
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelChaseDome
	waitmovement 0
	goto MtMoon_B2F_EventScript_StageJessieJames

MtMoon_B2F_EventScript_StageJessieJames::
	removeobject LOCALID_ROCKET_MEOWTH
	removeobject LOCALID_MIGUEL
	clearflag FLAG_0x0B5
	clearflag FLAG_0x0B6
	clearflag FLAG_0x0B7
	setobjectxy LOCALID_JESSIE, 5, 11
	setobjectxy LOCALID_ROCKET_MEOWTH, 6, 11
	setobjectxy LOCALID_JAMES, 7, 11
	addobject LOCALID_JESSIE
	addobject LOCALID_ROCKET_MEOWTH
	addobject LOCALID_JAMES
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketFaceUp
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketFaceUp
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketFaceUp
	waitmovement 0
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
	face_down
	step_end

MtMoon_B2F_Movement_MeowthEscapeHelixHeadStart::
	walk_up
	walk_left
	step_end

MtMoon_B2F_Movement_MeowthEscapeHelixCorridor::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MtMoon_B2F_Movement_MeowthEscapeDomeHeadStart::
	walk_up
	walk_left
	step_end

MtMoon_B2F_Movement_MeowthEscapeDomeCorridor::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MtMoon_B2F_Movement_MiguelPanic::
	walk_in_place_faster_down
	walk_in_place_faster_up
	walk_in_place_faster_down
	step_end

MtMoon_B2F_Movement_MiguelChaseHelix::
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MtMoon_B2F_Movement_MiguelChaseDome::
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MtMoon_B2F_EventScript_JessieJamesActor::
	goto MtMoon_B2F_EventScript_JessieJamesTrigger
	end

@ Fixed five-character faceoff:
@ Partner (6,9) + player (7,9), facing south.
@ Jessie (5,11) + Meowth (6,11) + James (7,11), facing north.
MtMoon_B2F_EventScript_JessieJamesTrigger::
	lockall
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	setobjectxy LOCALID_PLAYER, 7, 9
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 6, 9
	setobjectxy LOCALID_JESSIE, 5, 11
	setobjectxy LOCALID_ROCKET_MEOWTH, 6, 11
	setobjectxy LOCALID_JAMES, 7, 11
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerFaceTeamRocket
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerFaceTeamRocket
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketFacePlayer
	waitmovement 0
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerChallenge
	waitmovement OBJ_EVENT_ID_FOLLOWER
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	waitmoncry
	msgbox MtMoon_B2F_Text_JessieJamesStopRightThere
	msgbox MtMoon_B2F_Text_JessieJamesFossilIsTeamRockets
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	setvar VAR_LAST_TALKED, LOCALID_JESSIE
	trainerbattle_no_intro TRAINER_JESSIE_JAMES_MT_MOON, MtMoon_B2F_Text_JessieJamesDefeat
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 3
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerFaceTeamRocket
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerFaceTeamRocket
	applymovement LOCALID_JESSIE, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_ROCKET_MEOWTH, MtMoon_B2F_Movement_RocketFacePlayer
	applymovement LOCALID_JAMES, MtMoon_B2F_Movement_RocketFacePlayer
	waitmovement 0
	msgbox MtMoon_B2F_Text_JessieJamesBlastOff
	closemessage
	removeobject LOCALID_ROCKET_MEOWTH
	removeobject LOCALID_JESSIE
	removeobject LOCALID_JAMES
	setflag FLAG_0x0B5
	setflag FLAG_0x0B6
	setflag FLAG_0x0B7
	clearflag FLAG_0x0B8
	setobjectxy LOCALID_DROPPED_FOSSIL, 6, 11
	addobject LOCALID_DROPPED_FOSSIL
	setobjectxy LOCALID_MIGUEL, 10, 11
	addobject LOCALID_MIGUEL
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToDroppedFossil
	waitmovement LOCALID_MIGUEL
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerFaceMiguel
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerFaceMiguel
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFacePlayer
	waitmovement 0
	msgbox MtMoon_B2F_Text_MiguelYouRecoveredIt
	removeobject LOCALID_DROPPED_FOSSIL
	setflag FLAG_0x0B8
	goto_if_set FLAG_GOT_DOME_FOSSIL, MtMoon_B2F_EventScript_AwardHelixFossil
	giveitem ITEM_DOME_FOSSIL, 1
	setflag FLAG_GOT_DOME_FOSSIL
	goto MtMoon_B2F_EventScript_FinishRecoveredFossil

MtMoon_B2F_EventScript_AwardHelixFossil::
	giveitem ITEM_HELIX_FOSSIL, 1
	setflag FLAG_GOT_HELIX_FOSSIL

MtMoon_B2F_EventScript_FinishRecoveredFossil::
	applymovement LOCALID_PLAYER, MtMoon_B2F_Movement_PlayerFaceMiguel
	applymovement OBJ_EVENT_ID_FOLLOWER, MtMoon_B2F_Movement_PartnerFaceMiguel
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFacePlayer
	waitmovement 0
	msgbox MtMoon_B2F_Text_MiguelKeepRecoveredFossil
	setobjectxy LOCALID_MIGUEL, 13, 11
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelFaceRight
	waitmovement LOCALID_MIGUEL
	copyobjectxytoperm LOCALID_MIGUEL
	releaseall
	end

MtMoon_B2F_EventScript_DroppedFossil::
	msgbox MtMoon_B2F_Text_DroppedFossil, MSGBOX_SIGN
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

MtMoon_B2F_Movement_MiguelToDroppedFossil::
	walk_left
	walk_left
	walk_left
	face_left
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

MtMoon_B2F_Text_JessieJamesStopRightThere::
	.string "Stop right there!$"

MtMoon_B2F_Text_JessieJamesFossilIsTeamRockets::
	.string "That fossil is TEAM ROCKET's!\p"
	.string "Surrender now, or prepare to fight!$"

MtMoon_B2F_Text_JessieJamesDefeat::
	.string "A brat beat us?$"

MtMoon_B2F_Text_JessieJamesBlastOff::
	.string "TEAM ROCKET, blast off at the\n"
	.string "speed of light!$"

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
