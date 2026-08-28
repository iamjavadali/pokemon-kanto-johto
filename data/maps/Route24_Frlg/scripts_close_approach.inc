@ Golden Yellow Route 24: lane-aware close-approach choreography.
@ (7,7) is the left trigger; (8,7) is the right trigger.

Route24_EventScript_CharmanderCloseApproachLeft::
	goto_if_eq VAR_TEMP_E, 1, Route24_EventScript_CharmanderCloseApproachLeftSilent
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	setvar VAR_TEMP_E, 1
	applymovement LOCALID_PLAYER, Route24_Movement_CloseApproach
	waitmovement LOCALID_PLAYER
	delay 16
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_CharmanderCloseApproachLeftWeak
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_StageHealedReturn
	releaseall
	end

Route24_EventScript_CharmanderCloseApproachLeftWeak::
	@ Red is below Charmander at (7,6); Pikachu stages at (8,5), to its right.
	@ Charmander first acknowledges Red, then turns to Pikachu for the concern
	@ beat, then turns back to Red for the rescue decision.
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerStageBesideCharmander
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	delay 12
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerConcern
	waitmovement OBJ_EVENT_ID_FOLLOWER
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	msgbox Route24_Text_CharmanderLooksWeak
	msgbox Route24_Text_TakeCharmanderToCenter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderRescueDeclined
	goto Route24_EventScript_CharmanderRescueAccepted
	end

Route24_EventScript_CharmanderCloseApproachLeftSilent::
	end

Route24_EventScript_CharmanderCloseApproachRight::
	goto_if_eq VAR_TEMP_E, 1, Route24_EventScript_CharmanderCloseApproachRightSilent
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	lockall
	setvar VAR_TEMP_E, 1
	goto_if_eq VAR_0x40FC, 0, Route24_EventScript_CharmanderCloseApproachRightWeak
	goto_if_eq VAR_0x40FC, 2, Route24_EventScript_CharmanderCloseApproachRightHealed
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderCloseApproachRightWeak::
	@ Red moves straight north from (8,7) to (8,5), right of Charmander.
	@ Pikachu follows to (8,6), then steps left into (7,6) below Charmander.
	@ Charmander acknowledges Red on its right, turns down to Pikachu for the
	@ concern beat, then turns right again to address Red for the rescue choice.
	applymovement LOCALID_PLAYER, Route24_Movement_CloseApproachRightWeak
	waitmovement LOCALID_PLAYER
	delay 16
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerStageBelowCharmander
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	delay 12
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceDown
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerConcernUp
	waitmovement OBJ_EVENT_ID_FOLLOWER
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	msgbox Route24_Text_CharmanderLooksWeak
	msgbox Route24_Text_TakeCharmanderToCenter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route24_EventScript_CharmanderRightRescueDeclined
	goto Route24_EventScript_CharmanderRescueAccepted
	end

Route24_EventScript_CharmanderRightRescueDeclined::
	msgbox Route24_Text_CharmanderStillNeedsHelp
	@ Return Pikachu from (7,6) to (8,6), directly behind the right-lane Red.
	applymovement OBJ_EVENT_ID_FOLLOWER, Route24_Movement_PartnerReturnBehindPlayerRight
	waitmovement OBJ_EVENT_ID_FOLLOWER
	applymovement LOCALID_ROUTE24_YELLOW_CHARMANDER, Common_Movement_FaceRight
	waitmovement LOCALID_ROUTE24_YELLOW_CHARMANDER
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

Route24_EventScript_CharmanderCloseApproachRightHealed::
	@ Red moves left then north so the scene converges on Red (7,6) and
	@ Pikachu (7,7), keeping Damian's x=8 entrance lane completely clear.
	applymovement LOCALID_PLAYER, Route24_Movement_CloseApproachRightHealed
	waitmovement LOCALID_PLAYER
	delay 16
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	goto Route24_EventScript_StageHealedReturn
	end

Route24_EventScript_CharmanderCloseApproachRightSilent::
	end

Route24_Movement_CloseApproachRightWeak::
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

Route24_Movement_CloseApproachRightHealed::
	walk_left
	walk_up
	walk_in_place_faster_up
	step_end

Route24_Movement_PartnerStageBelowCharmander::
	walk_left
	walk_in_place_faster_up
	step_end

Route24_Movement_PartnerReturnBehindPlayerRight::
	walk_right
	walk_in_place_faster_up
	step_end

Route24_Movement_PartnerConcernUp::
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_up
	step_end
