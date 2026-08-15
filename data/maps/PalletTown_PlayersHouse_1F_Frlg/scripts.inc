PalletTown_PlayersHouse_1F_Frlg_MapScripts::
	.byte 0

PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchLeft::
	lockall
	goto_if_set FLAG_GOT_TRAINER_WATCH, PalletTown_PlayersHouse_1F_EventScript_MarkTrainerWatchSceneDone
	call PalletTown_PlayersHouse_1F_EventScript_MomAlert
	applymovement LOCALID_PLAYER, PalletTown_PlayersHouse_1F_Movement_PlayerToMomFromLeft
	waitmovement 0
	goto PalletTown_PlayersHouse_1F_EventScript_AutoTrainerWatch

PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchRight::
	lockall
	goto_if_set FLAG_GOT_TRAINER_WATCH, PalletTown_PlayersHouse_1F_EventScript_MarkTrainerWatchSceneDone
	call PalletTown_PlayersHouse_1F_EventScript_MomAlert
	applymovement LOCALID_PLAYER, PalletTown_PlayersHouse_1F_Movement_PlayerToMomFromRight
	waitmovement 0
	goto PalletTown_PlayersHouse_1F_EventScript_AutoTrainerWatch

PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchSide::
	lockall
	goto_if_set FLAG_GOT_TRAINER_WATCH, PalletTown_PlayersHouse_1F_EventScript_MarkTrainerWatchSceneDone
	call PalletTown_PlayersHouse_1F_EventScript_MomAlert
	applymovement LOCALID_PLAYER, PalletTown_PlayersHouse_1F_Movement_PlayerToMomFromSide
	waitmovement 0
	goto PalletTown_PlayersHouse_1F_EventScript_AutoTrainerWatch

PalletTown_PlayersHouse_1F_EventScript_MomAlert:
	playse SE_PIN
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, Common_Movement_ExclamationMark
	waitmovement 0
	delay 16
	msgbox PalletTown_PlayersHouse_1F_Text_MomStopBeforeLeaving
	closemessage
	return

PalletTown_PlayersHouse_1F_EventScript_AutoTrainerWatch::
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, Common_Movement_FacePlayer
	waitmovement 0
	call PalletTown_PlayersHouse_1F_EventScript_GiveTrainerWatch
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_1F, 1
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_MarkTrainerWatchSceneDone::
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_1F, 1
	releaseall
	end

PalletTown_PlayersHouse_1F_Movement_PlayerToMomFromLeft:
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

PalletTown_PlayersHouse_1F_Movement_PlayerToMomFromRight:
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

PalletTown_PlayersHouse_1F_Movement_PlayerToMomFromSide:
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

PalletTown_PlayersHouse_1F_EventScript_Mom::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_RIVAL_IN_OAKS_LAB, PalletTown_PlayersHouse_1F_EventScript_MomHeal
	goto_if_unset FLAG_GOT_TRAINER_WATCH, PalletTown_PlayersHouse_1F_EventScript_SetupTrainerWatch
	goto PalletTown_PlayersHouse_1F_EventScript_MomBeforeJourney
	end

PalletTown_PlayersHouse_1F_EventScript_SetupTrainerWatch::
	call PalletTown_PlayersHouse_1F_EventScript_GiveTrainerWatch
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_1F, 1
	goto PalletTown_PlayersHouse_1F_EventScript_MomBeforeJourney
	end

PalletTown_PlayersHouse_1F_EventScript_GiveTrainerWatch::
	msgbox PalletTown_PlayersHouse_1F_Text_MomTrainerWatchIntro
	closemessage
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message PalletTown_PlayersHouse_1F_Text_ReceivedTrainerWatch
	waitmessage
	waitfanfare
	setflag FLAG_GOT_TRAINER_WATCH
	specialvar VAR_RESULT, TrainerWatchCheckClock
	goto_if_eq VAR_RESULT, 1, PalletTown_PlayersHouse_1F_EventScript_TrainerWatchRtcReady
	goto_if_eq VAR_RESULT, 2, PalletTown_PlayersHouse_1F_EventScript_TrainerWatchSavedReady
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchNeedsManualSetup
	closemessage
	special StartTrainerWatchManualSetup
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchManualReady
	return

PalletTown_PlayersHouse_1F_EventScript_TrainerWatchRtcReady::
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchAutoSynced
	return

PalletTown_PlayersHouse_1F_EventScript_TrainerWatchSavedReady::
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchSavedTimeReady
	return

PalletTown_PlayersHouse_1F_EventScript_MomBeforeJourney::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale
	call_if_eq VAR_RESULT, FEMALE, PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouFemale
	closemessage
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale::
	msgbox PalletTown_PlayersHouse_1F_Text_AllBoysLeaveOakLookingForYou
	return

PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouFemale::
	msgbox PalletTown_PlayersHouse_1F_Text_AllGirlsLeaveOakLookingForYou
	return

PalletTown_PlayersHouse_1F_EventScript_MomHeal::
	msgbox PalletTown_PlayersHouse_1F_Text_YouShouldTakeQuickRest
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare
	release
	end

@ Displays special text if interacted with from side or back (which are normally inaccessible)
PalletTown_PlayersHouse_1F_EventScript_TV::
	lockall
	goto_if_eq VAR_FACING, DIR_NORTH, PalletTown_PlayersHouse_1F_EventScript_TVScreen
	msgbox PalletTown_PlayersHouse_1F_Text_OopsWrongSide
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreen::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenMale
	call_if_eq VAR_RESULT, FEMALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreenMale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVFourBoysOnRailroad
	return

PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVGirlOnBrickRoad
	return

PalletTown_PlayersHouse_1F_Text_AllBoysLeaveOakLookingForYou::
	.string "MOM: …Right.\n"
	.string "All boys leave home someday.\l"
	.string "It said so on TV.\p"
	.string "Oh, yes. PROF. OAK, next door, was\n"
	.string "looking for you.$"

PalletTown_PlayersHouse_1F_Text_AllGirlsLeaveOakLookingForYou::
	.string "MOM: …Right.\n"
	.string "All girls dream of traveling.\l"
	.string "It said so on TV.\p"
	.string "Oh, yes. PROF. OAK, next door, was\n"
	.string "looking for you.$"

PalletTown_PlayersHouse_1F_Text_YouShouldTakeQuickRest::
	.string "MOM: {PLAYER}!\n"
	.string "You should take a quick rest.$"

PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare::
	.string "MOM: Oh, good! You and your\n"
	.string "POKéMON are looking great.\l"
	.string "Take care now!$"

PalletTown_PlayersHouse_1F_Text_MovieOnTVFourBoysOnRailroad::
	.string "There's a movie on TV.\n"
	.string "Four boys are walking on railroad\l"
	.string "tracks.\p"
	.string "…I better go, too.$"

PalletTown_PlayersHouse_1F_Text_MovieOnTVGirlOnBrickRoad::
	.string "There's a movie on TV.\n"
	.string "A girl with her hair in pigtails is\l"
	.string "walking up a brick road.\p"
	.string "…I better go, too.$"

PalletTown_PlayersHouse_1F_Text_OopsWrongSide::
	.string "Oops, wrong side…$"

PalletTown_PlayersHouse_1F_Text_MomStopBeforeLeaving::
	.string "MOM: Wait, {PLAYER}!\n"
	.string "Before you go, there's one\l"
	.string "thing you need.$"

PalletTown_PlayersHouse_1F_Text_MomTrainerWatchIntro::
	.string "MOM: Before you leave, I have\n"
	.string "something for you.\p"
	.string "It's a TRAINER WATCH. It keeps\n"
	.string "the date and time on your journey.$"

PalletTown_PlayersHouse_1F_Text_ReceivedTrainerWatch::
	.string "{PLAYER} received the TRAINER WATCH!$"

PalletTown_PlayersHouse_1F_Text_TrainerWatchAutoSynced::
	.string "MOM: Perfect! Your TRAINER WATCH\n"
	.string "synchronized automatically.\p"
	.string "The world will follow the current\n"
	.string "date and time.$"

PalletTown_PlayersHouse_1F_Text_TrainerWatchSavedTimeReady::
	.string "MOM: Your TRAINER WATCH found its\n"
	.string "last saved time. You're all set.$"

PalletTown_PlayersHouse_1F_Text_TrainerWatchNeedsManualSetup::
	.string "MOM: It couldn't synchronize with\n"
	.string "a real-time clock.\p"
	.string "Set the date and time once, and the\n"
	.string "TRAINER WATCH will keep them for you.$"

PalletTown_PlayersHouse_1F_Text_TrainerWatchManualReady::
	.string "MOM: All set! If a real-time clock\n"
	.string "becomes available later, the WATCH\l"
	.string "will synchronize automatically.$"
