@ Golden Yellow Route 24 Charmander rescue states, stored in VAR_0x40FC.
@ 0 = WAITING_WEAK, 1 = TREATMENT_PENDING, 2 = HEALED_RETURNED,
@ 3 = DAMIAN_PRESENT, 4 = ADOPTED.
@ FLAG_0x0BB hides the Route 24 Charmander, FLAG_0x0BC hides Damian.
@ VAR_TEMP_E arms the close approach trigger. VAR_TEMP_F latches the distant
@ notice so the nine-tile notice zone only plays once per map load/story phase.

Route24_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route24_OnTransition
	.byte 0

Route24_OnTransition::
	call_if_eq VAR_MAP_SCENE_ROUTE24, 1, Route24_EventScript_ReconstructNuggetBridgeComplete
	call_if_eq VAR_0x40FC, 0, Route24_EventScript_StateWaitingWeak
	call_if_eq VAR_0x40FC, 1, Route24_EventScript_StateTreatmentPending
	call_if_eq VAR_0x40FC, 2, Route24_EventScript_StateHealedReturned
	call_if_eq VAR_0x40FC, 3, Route24_EventScript_StateDamianPresent
	call_if_eq VAR_0x40FC, 4, Route24_EventScript_StateAdopted
	end

Route24_EventScript_ReconstructNuggetBridgeComplete::
	settrainerflag TRAINER_BUG_CATCHER_CALE
	settrainerflag TRAINER_LASS_ALI
	settrainerflag TRAINER_YOUNGSTER_TIMMY
	settrainerflag TRAINER_LASS_RELI
	settrainerflag TRAINER_CAMPER_ETHAN
	settrainerflag TRAINER_TEAM_ROCKET_GRUNT_6
	return

Route24_EventScript_StateWaitingWeak::
	clearflag FLAG_0x0BB
	setflag FLAG_0x0BC
	return

Route24_EventScript_StateTreatmentPending::
	setflag FLAG_0x0BB
	setflag FLAG_0x0BC
	return

Route24_EventScript_StateHealedReturned::
	clearflag FLAG_0x0BB
	setflag FLAG_0x0BC
	return

Route24_EventScript_StateDamianPresent::
	clearflag FLAG_0x0BB
	clearflag FLAG_0x0BC
	addobject LOCALID_ROUTE24_DAMIAN
	setobjectxy LOCALID_ROUTE24_DAMIAN, 8, 5
	return

Route24_EventScript_StateAdopted::
	setflag FLAG_0x0BB
	clearflag FLAG_0x0BC
	addobject LOCALID_ROUTE24_DAMIAN
	setobjectxy LOCALID_ROUTE24_DAMIAN, 7, 5
	return

@ Direct interaction remains coordinate-safe for players who manually talk to
@ Charmander instead of entering through the close approach trigger.
Route24_EventScript_YellowCharmander::
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_CharmanderRescueInteraction
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_CharmanderReturnInteraction
	goto_if_eq VAR_0x40FC, 3, Route24_EventScript_CharmanderWaitsForPlayer
	releaseall
	end

@ Distant notice zone: (3..9,9) plus (9,8) and (9,7). It never moves Red or
@ Pikachu. Re-entering the zone silently re-arms the close trigger but does not
@ replay the cry until the map is loaded again.
Route24_EventScript_CharmanderNoticeZone::
	setvar VAR_TEMP_E, 0
	goto_if_eq VAR_TEMP_F, 1, Route24_EventScript_CharmanderNoticeZoneSilent
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	setvar VAR_TEMP_F, 1
	call Route24_EventScript_CharmanderNotice
	releaseall
	end

Route24_EventScript_CharmanderNoticeZoneSilent::
	end

Route24_EventScript_CharmanderNotice::
	@ Red and Pikachu look north first. Charmander then turns south, hops, and
	@ cries toward them. The custom Charmander movement is follower-safe scoped.
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	applymovement OBJ_EVENT_ID_FOLLOWER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Route24_Movement_CharmanderCallsForHelp
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	applymovement LOCALID_PLAYER, Common_Movement_Delay48
	waitmovement LOCALID_PLAYER
	call_if_eq VAR_0x40FC, 0, Route24_EventScript_CharmanderNoticeWeakText
	call_if_eq VAR_0x40FC, 2, Route24_EventScript_CharmanderNoticeHealthyText
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	return

Route24_EventScript_CharmanderNoticeWeakText::
	msgbox Route24_Text_NoticeCryingCharmander
	return

Route24_EventScript_CharmanderNoticeHealthyText::
	msgbox Route24_Text_NoticeHealthyCharmander
	return

@ Close approach gate. Red enters on (7,7), one tile south of the canonical
@ scene position. Normal scripted-follow movement handles one clean north step,
@ so Red ends at (7,6) and Pikachu ends directly behind at (7,7).
Route24_EventScript_CharmanderCloseApproach::
	goto_if_eq VAR_TEMP_E, 1, Route24_EventScript_CharmanderCloseApproachSilent
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	setvar VAR_TEMP_E, 1
	applymovement LOCALID_PLAYER, Route24_Movement_CloseApproach
	waitmovement LOCALID_PLAYER
	delay 16
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_StageInitialEncounter
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_StageHealedReturn
	releaseall
	end

Route24_EventScript_CharmanderCloseApproachSilent::
	end

Route24_EventScript_StageInitialEncounter::
	@ From the guaranteed (7,7) follower tile, Pikachu only needs this short,
	@ natural-looking three-step move to Charmander's right at (8,5).
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerStageBesideCharmander
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	goto Route24_EventScript_CharmanderPartnerConcern
	end

Route24_EventScript_StageHealedReturn::
	@ Red and Pikachu remain in a vertical line. Both face north and Pikachu stays
	@ completely stationary behind Red for the entire Damian sequence.
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	applymovement OBJ_EVENT_ID_FOLLOWER, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement 0
	goto Route24_EventScript_CharmanderReturnScene
	end

Route24_EventScript_CharmanderPartnerConcern::
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerConcern
	waitmovement OBJ_EVENT_ID_FOLLOWER
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry
	goto Route24_EventScript_CharmanderRescueText
	end

Route24_EventScript_CharmanderRescueInteraction::
	faceplayer
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	msgbox Route24_Text_CharmanderLooksWeak
	msgbox Route24_Text_TakeCharmanderToCenter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderDirectRescueDeclined
	goto Route24_EventScript_CharmanderRescueAccepted
	end

Route24_EventScript_CharmanderRescueText::
	msgbox Route24_Text_CharmanderLooksWeak
	msgbox Route24_Text_TakeCharmanderToCenter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderRescueDeclined
	goto Route24_EventScript_CharmanderRescueAccepted
	end

Route24_EventScript_CharmanderRescueAccepted::
	closemessage
	msgbox Route24_Text_PlayerLiftsCharmander
	setvar VAR_0x40FC, 1
	setflag FLAG_0x0BB
	removeobject LOCALID_ROUTE24_YELLOW_CHARMANDER
	fadescreen FADE_TO_BLACK
	warp MAP_CERULEAN_CITY_POKEMON_CENTER_1F, 7, 4
	waitstate
	end

Route24_EventScript_CharmanderRescueDeclined::
	msgbox Route24_Text_CharmanderStillNeedsHelp
	@ Red remains at (7,6). Pikachu returns to (7,7). VAR_TEMP_E remains set so
	@ Red can walk south across the close trigger without immediately retriggering.
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerReturnBehindPlayer
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderDirectRescueDeclined::
	msgbox Route24_Text_CharmanderStillNeedsHelp
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

@ Direct healed interaction never launches Damian from an arbitrary coordinate;
@ the close approach gate owns the canonical return choreography.
Route24_EventScript_CharmanderReturnInteraction::
	faceplayer
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	msgbox Route24_Text_CharmanderLooksHealthy
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderReturnScene::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	applymovement OBJ_EVENT_ID_FOLLOWER, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement 0
	msgbox Route24_Text_CharmanderLooksHealthy

	call Route24_EventScript_DamianEnterRightLane
	setvar VAR_0x40FC, 3

	@ From this point until the scene releases, Red and Pikachu remain NORTH.
	@ Damian and Charmander alone change facing according to who is speaking.
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	msgbox Route24_Text_DamianFoundCharmander

	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceDown
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement 0
	msgbox Route24_Text_DamianNoticesPlayer
	msgbox Route24_Text_DamianConfession

	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceLeft
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement 0
	msgbox Route24_Text_DamianCallsCharmander
	delay 24

	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Route24_Movement_CharmanderChoosesPlayer
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceLeft
	waitmovement LOCALID_ROUTE24_DAMIAN
	msgbox Route24_Text_CharmanderChoosesPlayer

	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceDown
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement 0
	msgbox Route24_Text_DamianUnderstands
	msgbox Route24_Text_DamianOffer
	goto Route24_EventScript_CharmanderAdoptionPrompt
	end

Route24_EventScript_DamianEnterRightLane::
	setobjectxyperm LOCALID_ROUTE24_DAMIAN, 8, 9
	clearflag FLAG_0x0BC
	addobject LOCALID_ROUTE24_DAMIAN
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_ROUTE24_DAMIAN, Route24_Movement_DamianEnterRightLane
	waitmovement LOCALID_ROUTE24_DAMIAN
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	return

Route24_EventScript_Damian::
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	faceplayer
	goto_if_eq VAR_0x40FC, 4, Route24_EventScript_DamianPostAdoption
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	goto Route24_EventScript_CharmanderAdoptionPrompt
	end

Route24_EventScript_CharmanderWaitsForPlayer::
	faceplayer
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	msgbox Route24_Text_CharmanderWaitsExpectantly
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderAdoptionPrompt::
	msgbox Route24_Text_TakeCharmanderWithYou, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderAdoptionDeclined
	setvar VAR_TEMP_1, SPECIES_CHARMANDER
	givemon SPECIES_CHARMANDER, 10
	goto_if_eq VAR_RESULT, 0, Route24_EventScript_ReceiveCharmanderParty
	goto_if_eq VAR_RESULT, 1, Route24_EventScript_ReceiveCharmanderPC
	goto_if_eq VAR_RESULT, 2, Route24_EventScript_CharmanderNoStorage
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderNoStorage::
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	goto Common_EventScript_NoMoreRoomForPokemon
	end

Route24_EventScript_CommitCharmanderAdoption::
	setvar VAR_0x40FC, 4
	setflag FLAG_0x0BB
	removeobject LOCALID_ROUTE24_YELLOW_CHARMANDER
	return

Route24_EventScript_ReceiveCharmanderParty::
	call Route24_EventScript_CommitCharmanderAdoption
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message Route24_Text_ObtainedCharmander
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_FinishCharmanderAdoption
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto Route24_EventScript_FinishCharmanderAdoption
	end

Route24_EventScript_ReceiveCharmanderPC::
	call Route24_EventScript_CommitCharmanderAdoption
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message Route24_Text_ObtainedCharmander
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto Route24_EventScript_CharmanderTransferredToPC
	end

Route24_EventScript_CharmanderTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto Route24_EventScript_FinishCharmanderAdoption
	end

Route24_EventScript_FinishCharmanderAdoption::
	@ Damian moves left into Charmander's former tile. Red and Pikachu are already
	@ facing north, so no player/follower facing change is needed here.
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_ROUTE24_DAMIAN, Route24_Movement_DamianStepIntoCharmanderSpot
	waitmovement LOCALID_ROUTE24_DAMIAN
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Route24_Text_DamianAfterAdoption
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderAdoptionDeclined::
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceDown
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement 0
	msgbox Route24_Text_DamianTakeYourTime
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_DamianPostAdoption::
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FacePlayer
	waitmovement LOCALID_ROUTE24_DAMIAN
	msgbox Route24_Text_DamianPostAdoption
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_Movement_CloseApproach::
	walk_up
	walk_in_place_faster_up
	step_end

Route24_Movement_CharmanderCallsForHelp::
	walk_in_place_faster_down
	delay_8
	jump_in_place_down
	step_end

Route24_Movement_PartnerStageBesideCharmander::
	walk_right
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

Route24_Movement_PartnerReturnBehindPlayer::
	walk_down
	walk_down
	walk_left
	walk_in_place_faster_up
	step_end

Route24_Movement_PartnerConcern::
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_left
	step_end

Route24_Movement_DamianEnterRightLane::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

Route24_Movement_DamianStepIntoCharmanderSpot::
	walk_left
	walk_in_place_faster_down
	step_end

Route24_Movement_CharmanderChoosesPlayer::
	walk_in_place_faster_down
	delay_16
	jump_in_place_down
	step_end

Route24_EventScript_Rocket::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_ROUTE24, 1, Route24_EventScript_RocketPostBattle
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNugget
	call Route24_EventScript_BattleRocket
	release
	end

Route24_EventScript_NoRoomForNugget::
	msgbox Route24_Text_YouDontHaveAnyRoom
	release
	end

Route24_EventScript_RocketPostBattle::
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	release
	end

Route24_EventScript_RocketTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTriggerRight::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketApproachPlayer
	call_if_eq VAR_TEMP_1, 1, Route24_EventScript_RocketMotionToPlayer
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNuggetTrigger
	call Route24_EventScript_BattleRocket
	releaseall
	end

Route24_EventScript_BattleRocket::
	giveitem_msg Route24_Text_ReceivedNuggetFromMysteryTrainer, ITEM_NUGGET
	message Route24_Text_JoinTeamRocket
	waitmessage
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	waitbuttonpress
	setvar VAR_LAST_TALKED, LOCALID_ROUTE24_ROCKET
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_6, Route24_Text_RocketDefeat
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	setvar VAR_MAP_SCENE_ROUTE24, 1
	return

Route24_EventScript_RocketApproachPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketApproachPlayer
	waitmovement 0
	return

Route24_EventScript_RocketMotionToPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

Route24_EventScript_NoRoomForNuggetTrigger::
	msgbox Route24_Text_YouDontHaveAnyRoom
	closemessage
	applymovement LOCALID_PLAYER, Route24_Movement_WalkDown
	waitmovement 0
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketWalkBackToPos
	release
	end

Route24_EventScript_RocketWalkBackToPos::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketWalkBackToPos
	waitmovement 0
	return

Route24_Movement_RocketApproachPlayer::
	walk_left
	step_end

Route24_Movement_RocketWalkBackToPos::
	walk_right
	walk_in_place_faster_left
	step_end

Route24_Movement_WalkDown::
	walk_down
	step_end

Route24_Text_NoticeCryingCharmander::
	.string "A CHARMANDER is crying nearby.$"

Route24_Text_NoticeHealthyCharmander::
	.string "CHARMANDER is calling out up ahead.$"

Route24_Text_CharmanderLooksWeak::
	.string "CHARMANDER lets out a weak cry.\p"
	.string "Its flame is low, and it can barely\n"
	.string "stand.$"

Route24_Text_TakeCharmanderToCenter::
	.string "CHARMANDER needs help.\p"
	.string "Take it to the POKéMON CENTER?$"

Route24_Text_PlayerLiftsCharmander::
	.string "You gently lift the weakened\n"
	.string "CHARMANDER.\p"
	.string "It needs help right away!$"

Route24_Text_CharmanderStillNeedsHelp::
	.string "CHARMANDER stays where it is.\p"
	.string "It still looks like it needs help.$"

Route24_Text_CharmanderLooksHealthy::
	.string "CHARMANDER looks much better!\p"
	.string "It happily recognizes you.$"

Route24_Text_DamianFoundCharmander::
	.string "DAMIAN: CHARMANDER!\p"
	.string "There you are!$"

Route24_Text_DamianNoticesPlayer::
	.string "DAMIAN: Hey...\p"
	.string "Were you the one who helped it?$"

Route24_Text_DamianConfession::
	.string "I left CHARMANDER here.\p"
	.string "I kept losing battles, and I blamed\n"
	.string "it for being too weak.\p"
	.string "But leaving it here like that...\n"
	.string "That wasn't CHARMANDER's fault.$"

Route24_Text_DamianCallsCharmander::
	.string "DAMIAN: Come on, CHARMANDER.\p"
	.string "Let's go.$"

Route24_Text_CharmanderChoosesPlayer::
	.string "CHARMANDER turns back toward you.\p"
	.string "It looks like it wants to stay\n"
	.string "with you.$"

Route24_Text_DamianUnderstands::
	.string "DAMIAN: ...I get it.\p"
	.string "After what I did, you trust this\n"
	.string "TRAINER more than me.$"

Route24_Text_DamianOffer::
	.string "I'm not good at raising POKéMON.\p"
	.string "But you took care of CHARMANDER\n"
	.string "when it needed someone.\p"
	.string "If you promise you'll care for it,\n"
	.string "it's yours.$"

Route24_Text_TakeCharmanderWithYou::
	.string "Will you take CHARMANDER?$"

Route24_Text_CharmanderWaitsExpectantly::
	.string "CHARMANDER looks at you\n"
	.string "expectantly.$"

Route24_Text_ObtainedCharmander::
	.string "{PLAYER} received CHARMANDER!$"

Route24_Text_DamianTakeYourTime::
	.string "DAMIAN: I understand.\p"
	.string "CHARMANDER will wait here until\n"
	.string "you're ready.$"

Route24_Text_DamianAfterAdoption::
	.string "DAMIAN: Take good care of it.\p"
	.string "CHARMANDER chose you.$"

Route24_Text_DamianPostAdoption::
	.string "DAMIAN: CHARMANDER is better off\n"
	.string "with you.\p"
	.string "Please take good care of it.$"

Route24_Text_JustEarnedFabulousPrize::
	.string "Congratulations! You beat our\n"
	.string "five contest TRAINERS!\p"
	.string "You just earned a fabulous prize!$"

Route24_Text_ReceivedNuggetFromMysteryTrainer::
	.string "{PLAYER} received a NUGGET\n"
	.string "from the mystery TRAINER!$"

Route24_Text_YouDontHaveAnyRoom::
	.string "You don't have any room!$"

Route24_Text_JoinTeamRocket::
	.string "By the way, how would you like to\n"
	.string "join TEAM ROCKET?\p"
	.string "We're a group of professional\n"
	.string "criminals specializing in POKéMON!\p"
	.string "Want to join?\p"
	.string "Are you sure?\p"
	.string "Come on, join us!\p"
	.string "I'm telling you to join!\p"
	.string "…Okay, you need convincing!\p"
	.string "I'll make you an offer you can't\n"
	.string "refuse!$"

Route24_Text_RocketDefeat::
	.string "Arrgh!\n"
	.string "You are good!$"

Route24_Text_YoudBecomeTopRocketLeader::
	.string "With your ability, you'd become\n"
	.string "a top leader in TEAM ROCKET.\p"
	.string "Come on, think of the opportunity!\n"
	.string "Don't let this chance go to waste.$"

Route24_Text_ShaneIntro::
	.string "I saw your feat from the grass!$"

Route24_Text_ShaneDefeat::
	.string "I thought not!$"

Route24_Text_ShanePostBattle::
	.string "I hid because the people on the\n"
	.string "bridge frightened me.$"

Route24_Text_EthanIntro::
	.string "Okay! I'm No. 5!\n"
	.string "I'll stomp you!$"

Route24_Text_EthanDefeat::
	.string "Whoa!\n"
	.string "Too much!$"

Route24_Text_EthanPostBattle::
	.string "I did my best. I have no regrets!$"

Route24_Text_ReliIntro::
	.string "I'm No. 4!\n"
	.string "Getting tired?$"

Route24_Text_ReliDefeat::
	.string "I lost, too!$"

Route24_Text_ReliPostBattle::
	.string "I did my best, so I've no regrets!$"

Route24_Text_TimmyIntro::
	.string "Here's No. 3!\n"
	.string "I won't be easy!$"

Route24_Text_TimmyDefeat::
	.string "Ow!\n"
	.string "Stomped flat!$"

Route24_Text_TimmyPostBattle::
	.string "I did my best. I have no regrets!$"

Route24_Text_AliIntro::
	.string "I'm second!\n"
	.string "Now it's serious!$"

Route24_Text_AliDefeat::
	.string "How could I lose?$"

Route24_Text_AliPostBattle::
	.string "I did my best. I've no regrets!$"

Route24_Text_CaleIntro::
	.string "People call this the NUGGET\n"
	.string "BRIDGE!\p"
	.string "Beat us five TRAINERS and win\n"
	.string "a fabulous prize!\p"
	.string "Think you got what it takes?$"

Route24_Text_CaleDefeat::
	.string "Whoo!\n"
	.string "Good stuff!$"

Route24_Text_CalePostBattle::
	.string "I did my best. I have no regrets!$"
