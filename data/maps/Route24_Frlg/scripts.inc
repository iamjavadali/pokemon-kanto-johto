@ Golden Yellow Route 24 Charmander rescue states, stored in VAR_0x40FC.
@ 0 = WAITING_WEAK, 1 = TREATMENT_PENDING, 2 = HEALED_RETURNED,
@ 3 = DAMIAN_PRESENT, 4 = ADOPTED.
@ FLAG_0x0BB hides the Route 24 Charmander, FLAG_0x0BC hides Damian.

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

@ Direct interaction never assumes a specific player/follower coordinate.
@ The three Nugget Bridge approach triggers below own the cinematic staging.
Route24_EventScript_YellowCharmander::
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_CharmanderRescueInteraction
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_CharmanderReturnInteraction
	goto_if_eq VAR_0x40FC, 3, Route24_EventScript_CharmanderWaitsForPlayer
	releaseall
	end

@ The rescue gate is the three walkable tiles at the north end of Nugget Bridge:
@ (10,9), (11,9), and (12,9). Stepping on one asks whether Red wants to
@ approach. NO moves Red one tile south off the trigger row. YES uses explicit
@ player and Partner Pikachu paths so both actors finish on exact scene tiles.
Route24_EventScript_CharmanderApproachLeft::
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	setvar VAR_TEMP_2, 0
	goto Route24_EventScript_CharmanderApproachGate
	end

Route24_EventScript_CharmanderApproachCenter::
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	setvar VAR_TEMP_2, 1
	goto Route24_EventScript_CharmanderApproachGate
	end

Route24_EventScript_CharmanderApproachRight::
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	setvar VAR_TEMP_2, 2
	goto Route24_EventScript_CharmanderApproachGate
	end

Route24_EventScript_CharmanderApproachGate::
	call Route24_EventScript_CharmanderNotice
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_CharmanderApproachPromptWeak
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_CharmanderApproachPromptHealthy
	goto Route24_EventScript_CharmanderApproachDeclined
	end

Route24_EventScript_CharmanderApproachPromptWeak::
	msgbox Route24_Text_ApproachCryingCharmander, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderApproachDeclined
	closemessage
	goto Route24_EventScript_CharmanderCommitApproach
	end

Route24_EventScript_CharmanderApproachPromptHealthy::
	msgbox Route24_Text_ApproachHealthyCharmander, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderApproachDeclined
	closemessage
	goto Route24_EventScript_CharmanderCommitApproach
	end

Route24_EventScript_CharmanderApproachDeclined::
	closemessage
	@ Restore Charmander's normal east-facing idle pose, then step Red off the
	@ adjacent trigger row. Pikachu copies only this short retreat movement.
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Route24_Movement_DeclineStepSouth
	waitmovement LOCALID_PLAYER
	delay 16
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderCommitApproach::
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_CommitInitialApproach
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_CommitHealedApproach
	releaseall
	end

@ The follower is normalized one tile south of the triggering player tile.
@ SAFE is enabled only while the two movement scripts are assigned, preventing
@ the player's custom movement from recalling Pikachu without allowing Pikachu
@ to copy Red while it is already executing its own deterministic route.
Route24_EventScript_CommitInitialApproach::
	call_if_eq VAR_TEMP_2, 0, Route24_EventScript_InitialApproachFromLeft
	call_if_eq VAR_TEMP_2, 1, Route24_EventScript_InitialApproachFromCenter
	call_if_eq VAR_TEMP_2, 2, Route24_EventScript_InitialApproachFromRight
	waitmovement LOCALID_PLAYER
	waitmovement OBJ_EVENT_ID_FOLLOWER
	goto Route24_EventScript_StageInitialEncounter
	end

Route24_EventScript_CommitHealedApproach::
	call_if_eq VAR_TEMP_2, 0, Route24_EventScript_HealedApproachFromLeft
	call_if_eq VAR_TEMP_2, 1, Route24_EventScript_HealedApproachFromCenter
	call_if_eq VAR_TEMP_2, 2, Route24_EventScript_HealedApproachFromRight
	waitmovement LOCALID_PLAYER
	waitmovement OBJ_EVENT_ID_FOLLOWER
	goto Route24_EventScript_StageHealedReturn
	end

Route24_EventScript_InitialApproachFromLeft::
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 10, 10
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Route24_Movement_ApproachFromLeft
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerInitialFromLeft
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	return

Route24_EventScript_InitialApproachFromCenter::
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 11, 10
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Route24_Movement_ApproachFromCenter
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerInitialFromCenter
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	return

Route24_EventScript_InitialApproachFromRight::
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 12, 10
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Route24_Movement_ApproachFromRight
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerInitialFromRight
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	return

Route24_EventScript_HealedApproachFromLeft::
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 10, 10
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Route24_Movement_ApproachFromLeft
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerHealedFromLeft
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	return

Route24_EventScript_HealedApproachFromCenter::
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 11, 10
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Route24_Movement_ApproachFromCenter
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerHealedFromCenter
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	return

Route24_EventScript_HealedApproachFromRight::
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 12, 10
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Route24_Movement_ApproachFromRight
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerHealedFromRight
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	return

Route24_EventScript_StageInitialEncounter::
	@ Red is now at (7,6), Pikachu is at (8,5), facing Charmander from its right.
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	goto Route24_EventScript_CharmanderPartnerConcern
	end

Route24_EventScript_StageHealedReturn::
	@ Red is at (7,6) and Pikachu is exactly behind Red at (7,7), facing north.
	@ Pikachu remains stationary there for the entire Damian scene.
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	goto Route24_EventScript_CharmanderReturnScene
	end

Route24_EventScript_CharmanderNotice::
	@ Charmander idles facing east. The trigger visibly turns it south toward the
	@ bridge while Red and Pikachu turn north toward its cry. This beat is shared
	@ by both the weak first encounter and the healed return encounter.
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	applymovement OBJ_EVENT_ID_FOLLOWER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	applymovement LOCALID_PLAYER, Common_Movement_Delay48
	waitmovement LOCALID_PLAYER
	return

Route24_EventScript_CharmanderPartnerConcern::
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerConcern
	waitmovement OBJ_EVENT_ID_FOLLOWER
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry
	goto Route24_EventScript_CharmanderRescueText
	end

@ Talking directly to Charmander after declining is intentionally coordinate-safe:
@ no Pikachu restaging is attempted from an unknown roaming position.
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
	@ Canonical staged decline: return Pikachu from (8,5) to (7,7), restore
	@ Charmander's east-facing idle pose, then hand control back to the follower.
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

@ Direct interaction with healed Charmander never launches Damian from an
@ arbitrary coordinate. The three-tile approach gate owns that choreography.
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
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	msgbox Route24_Text_CharmanderLooksHealthy

	@ Damian approaches straight up the open right lane to (8,5), directly right
	@ of Charmander. Pikachu remains stationary at (7,7) throughout this scene.
	call Route24_EventScript_DamianEnterRightLane
	setvar VAR_0x40FC, 3

	@ Damian calls to Charmander: they face each other, while Red watches north.
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox Route24_Text_DamianFoundCharmander

	@ Damian turns down toward Red; Red turns right toward Damian. Charmander
	@ continues watching Damian from the left.
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement 0
	msgbox Route24_Text_DamianNoticesPlayer
	msgbox Route24_Text_DamianConfession

	@ Damian asks Charmander to come with him.
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceLeft
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox Route24_Text_DamianCallsCharmander
	delay 24

	@ Charmander's custom choice animation is the only moving actor here. Keep
	@ the follower-safe flag on only while that custom movement is running.
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Route24_Movement_CharmanderChoosesPlayer
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceLeft
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox Route24_Text_CharmanderChoosesPlayer

	@ Damian addresses Red for the handoff.
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
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
	@ With Charmander removed, Damian moves left from (8,5) into its old (7,5)
	@ tile, then faces down toward Red. Red faces up only after SAFE is cleared.
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_ROUTE24_DAMIAN, Route24_Movement_DamianStepIntoCharmanderSpot
	waitmovement LOCALID_ROUTE24_DAMIAN
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement LOCALID_PLAYER
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Route24_Text_DamianAfterAdoption
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderAdoptionDeclined::
	applymovement LOCALID_ROUTE24_DAMIAN, Common_Movement_FaceDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
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

@ Player routes from the three trigger tiles all finish Red at (7,6).
Route24_Movement_ApproachFromLeft::
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route24_Movement_ApproachFromCenter::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route24_Movement_ApproachFromRight::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route24_Movement_DeclineStepSouth::
	walk_down
	walk_in_place_faster_up
	step_end

@ Initial encounter: Pikachu travels independently through the x=8 lane and
@ finishes exactly right of Charmander at (8,5), facing left.
Route24_Movement_PartnerInitialFromLeft::
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

Route24_Movement_PartnerInitialFromCenter::
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

Route24_Movement_PartnerInitialFromRight::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

@ Healed return: Pikachu travels through x=8, then steps left into the exact
@ follower staging tile (7,7) behind Red and faces north before Damian enters.
Route24_Movement_PartnerHealedFromLeft::
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_faster_up
	step_end

Route24_Movement_PartnerHealedFromCenter::
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_faster_up
	step_end

Route24_Movement_PartnerHealedFromRight::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_faster_up
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

Route24_Text_ApproachCryingCharmander::
	.string "A CHARMANDER is crying nearby.\p"
	.string "Approach it?$"

Route24_Text_ApproachHealthyCharmander::
	.string "CHARMANDER is waiting up ahead.\p"
	.string "Approach it?$"

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
