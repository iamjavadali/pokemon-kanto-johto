@ Golden Yellow Route 24: two-stage, lane-aware Charmander presentation.
@ (7,7) is the left trigger; (8,7) is the right trigger. Neither trigger moves
@ Red or opens dialogue. The player manually presses A on Charmander afterward.

Route24_EventScript_CharmanderCloseApproachLeft::
	goto_if_eq VAR_TEMP_E, 1, Route24_EventScript_CharmanderCloseApproachSilent
	setvar VAR_TEMP_E, 1
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_PresentWeakFromLeft
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_PresentHealedFromLeft
	end

Route24_EventScript_CharmanderCloseApproachRight::
	goto_if_eq VAR_TEMP_E, 1, Route24_EventScript_CharmanderCloseApproachSilent
	setvar VAR_TEMP_E, 1
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_PresentWeakFromRight
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_PresentHealedFromRight
	end

Route24_EventScript_CharmanderCloseApproachSilent::
	end

Route24_EventScript_PresentWeakFromLeft::
	lockall
	call Route24_EventScript_BeginCharmanderCameraPan
	callnative GoldenYellow_ParkRoute24PartnerFollower
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerStageWeakFromLeft
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	call Route24_EventScript_CharmanderPartnerConcern
	goto Route24_EventScript_EndCharmanderCameraPan

Route24_EventScript_PresentWeakFromRight::
	lockall
	call Route24_EventScript_BeginCharmanderCameraPan
	callnative GoldenYellow_ParkRoute24PartnerFollower
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerStageWeakFromRight
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	call Route24_EventScript_CharmanderPartnerConcern
	goto Route24_EventScript_EndCharmanderCameraPan

Route24_EventScript_PresentHealedFromLeft::
	lockall
	call Route24_EventScript_BeginCharmanderCameraPan
	callnative GoldenYellow_ParkRoute24PartnerFollower
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerStageHealedFromLeft
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceUp
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	call Route24_EventScript_CharmanderPartnerConcern
	goto Route24_EventScript_EndCharmanderCameraPan

Route24_EventScript_PresentHealedFromRight::
	lockall
	call Route24_EventScript_BeginCharmanderCameraPan
	callnative GoldenYellow_ParkRoute24PartnerFollower
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerStageHealedFromRight
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceUp
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	call Route24_EventScript_CharmanderPartnerConcern
	goto Route24_EventScript_EndCharmanderCameraPan

Route24_EventScript_BeginCharmanderCameraPan::
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	special SpawnCameraObject
	applymovement LOCALID_CAMERA, Route24_Movement_CameraPanToCharmander
	waitmovement 0
	delay 16
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Route24_Movement_CharmanderCallsForHelp
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement LOCALID_PLAYER
	delay 16
	return

Route24_EventScript_EndCharmanderCameraPan::
	delay 16
	applymovement LOCALID_CAMERA, Route24_Movement_CameraPanBackFromCharmander
	waitmovement 0
	special RemoveCameraObject
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

@ Initial encounter: Pikachu parks east of Charmander at (8,5), facing west.
Route24_Movement_PartnerStageWeakFromLeft::
	walk_right
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

Route24_Movement_PartnerStageWeakFromRight::
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

@ Healed encounter: Pikachu parks north of Charmander at (7,4), facing south.
@ Separate paths keep it from crossing Charmander's occupied center tile.
Route24_Movement_PartnerStageHealedFromLeft::
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_in_place_faster_down
	step_end

Route24_Movement_PartnerStageHealedFromRight::
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end
