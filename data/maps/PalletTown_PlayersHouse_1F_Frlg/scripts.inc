PalletTown_PlayersHouse_1F_Frlg_MapScripts::
	.byte 0

PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatch::
	lockall
	goto_if_set FLAG_GOT_TRAINER_WATCH, PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchDone
	msgbox PalletTown_PlayersHouse_1F_Text_MomTrainerWatchIntro
	closemessage
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message PalletTown_PlayersHouse_1F_Text_ReceivedTrainerWatch
	waitmessage
	waitfanfare
	setflag FLAG_GOT_TRAINER_WATCH
	specialvar VAR_RESULT, TrainerWatchCheckClock
	goto_if_eq VAR_RESULT, 1, PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchRtcReady
	goto_if_eq VAR_RESULT, 2, PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchSavedReady
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchNeedsManualSetup
	closemessage
	special StartTrainerWatchManualSetup
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchManualReady
	goto PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchDone

PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchRtcReady::
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchAutoSynced
	goto PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchDone

PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchSavedReady::
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchSavedTimeReady

PalletTown_PlayersHouse_1F_EventScript_EnsureTrainerWatchDone::
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_Mom::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_RIVAL_IN_OAKS_LAB, PalletTown_PlayersHouse_1F_EventScript_MomHeal
	goto_if_unset FLAG_GOT_TRAINER_WATCH, PalletTown_PlayersHouse_1F_EventScript_SetupTrainerWatch
	goto PalletTown_PlayersHouse_1F_EventScript_MomBeforeJourney
	end

PalletTown_PlayersHouse_1F_EventScript_SetupTrainerWatch::
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
	goto PalletTown_PlayersHouse_1F_EventScript_MomBeforeJourney
	end

PalletTown_PlayersHouse_1F_EventScript_TrainerWatchRtcReady::
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchAutoSynced
	goto PalletTown_PlayersHouse_1F_EventScript_MomBeforeJourney
	end

PalletTown_PlayersHouse_1F_EventScript_TrainerWatchSavedReady::
	msgbox PalletTown_PlayersHouse_1F_Text_TrainerWatchSavedTimeReady
	goto PalletTown_PlayersHouse_1F_EventScript_MomBeforeJourney
	end

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

PalletTown_PlayersHouse_1F_Text_MomTrainerWatchIntro::
	.string "MOM: Before you go, I have\n"
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
