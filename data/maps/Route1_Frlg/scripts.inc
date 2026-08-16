Route1_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route1_EventScript_YellowOakOnTransition
	.byte 0

Route1_EventScript_YellowOakOnTransition:
	goto_if_ne VAR_MAP_SCENE_PALLET_TOWN_OAK, 0, Route1_EventScript_YellowOakTransitionDone
	setflag FLAG_HIDE_OAK_IN_PALLET_TOWN
	setflag FLAG_HIDE_ROUTE1_YELLOW_OAK
	setflag FLAG_HIDE_PALLET_WILD_PIKACHU
Route1_EventScript_YellowOakTransitionDone:
	end

Route1_EventScript_YellowOakTriggerLeft::
	lockall
	setvar VAR_0x8008, 0
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	waitmovement 0
	goto Route1_EventScript_YellowOakTrigger

Route1_EventScript_YellowOakTriggerRight::
	lockall
	setvar VAR_0x8008, 1
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	waitmovement 0

Route1_EventScript_YellowOakTrigger:
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	delay 16
	famechecker FAMECHECKER_OAK, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	textcolor NPC_TEXT_COLOR_MALE
	delay 20
	playbgm MUS_RG_OAK, 0
	message PalletTown_Text_OakDontGoOut
	waitmessage
	delay 55
	closemessage
	clearflag FLAG_HIDE_ROUTE1_YELLOW_OAK
	addobject LOCALID_ROUTE1_YELLOW_OAK
	goto_if_eq VAR_0x8008, 1, Route1_EventScript_YellowOakApproachRight
	applymovement LOCALID_ROUTE1_YELLOW_OAK, Route1_Movement_OakEnterLeft
	waitmovement 0
	goto Route1_EventScript_YellowOakStageReady

Route1_EventScript_YellowOakApproachRight:
	applymovement LOCALID_ROUTE1_YELLOW_OAK, Route1_Movement_OakEnterRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0

Route1_EventScript_YellowOakStageReady:
	delay 16
	msgbox PalletTown_Text_OakGrassUnsafeNeedMon
	closemessage
	goto_if_eq VAR_0x8008, 1, Route1_EventScript_YellowPikachuRight

Route1_EventScript_YellowPikachuLeft:
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	applymovement LOCALID_ROUTE1_YELLOW_OAK, Common_Movement_FaceRight
	waitmovement 0
	clearflag FLAG_HIDE_PALLET_WILD_PIKACHU
	addobject LOCALID_ROUTE1_YELLOW_PIKACHU
	delay 16
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	waitmoncry
	delay 24
	setflag FLAG_HIDE_PALLET_WILD_PIKACHU
	removeobject LOCALID_ROUTE1_YELLOW_PIKACHU
	goto Route1_EventScript_YellowPikachuCapture

Route1_EventScript_YellowPikachuRight:
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	applymovement LOCALID_ROUTE1_YELLOW_OAK, Common_Movement_FaceDown
	waitmovement 0
	clearflag FLAG_HIDE_PALLET_WILD_PIKACHU
	addobject LOCALID_ROUTE1_YELLOW_PIKACHU_RIGHT
	delay 16
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	waitmoncry
	delay 24
	setflag FLAG_HIDE_PALLET_WILD_PIKACHU
	removeobject LOCALID_ROUTE1_YELLOW_PIKACHU_RIGHT

Route1_EventScript_YellowPikachuCapture:
	special StartYellowPikachuCaptureBattle
	waitstate
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox PalletTown_Text_OakWhew
	msgbox PalletTown_Text_OakComeWithMeYellow
	closemessage
	delay 12
	goto_if_eq VAR_0x8008, 1, Route1_EventScript_YellowEscortWalkRight
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 4
	call Route1_EventScript_YellowEscortToPalletEdge
	end

Route1_EventScript_YellowEscortWalkRight::
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 5
	applymovement LOCALID_ROUTE1_YELLOW_OAK, Route1_Movement_OakMoveBehindPlayerRight
	waitmovement LOCALID_ROUTE1_YELLOW_OAK
	call Route1_EventScript_YellowEscortToPalletEdge
	end

Route1_EventScript_YellowEscortToPalletEdge::
	applymovement LOCALID_PLAYER, Route1_Movement_PlayerEscortToPalletEdge
	applymovement LOCALID_ROUTE1_YELLOW_OAK, Route1_Movement_OakEscortToPalletEdge
	waitmovement LOCALID_PLAYER
	setflag FLAG_HIDE_ROUTE1_YELLOW_OAK
	removeobject LOCALID_ROUTE1_YELLOW_OAK
	applymovement LOCALID_PLAYER, Route1_Movement_PlayerCrossIntoPallet
	waitmovement LOCALID_PLAYER
	return

Route1_Movement_PlayerEscortToPalletEdge::
	walk_down
	walk_down
	step_end

Route1_Movement_OakEscortToPalletEdge::
	walk_down
	walk_down
	step_end

Route1_Movement_PlayerCrossIntoPallet::
	walk_down
	step_end

Route1_Movement_OakMoveBehindPlayerRight:
	walk_down
	walk_right
	walk_in_place_faster_down
	step_end

Route1_Movement_OakEnterLeft:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route1_Movement_OakEnterRight:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_right
	step_end

Route1_EventScript_MartClerk::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POTION_ON_ROUTE_1, Route1_EventScript_AlreadyGotPotion
	msgbox Route1_Text_WorkAtPokeMartTakeSample
	textcolor NPC_TEXT_COLOR_NEUTRAL
	checkitemspace ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	bufferitemname STR_VAR_2, ITEM_POTION
	playfanfare MUS_LEVEL_UP
	message gText_ObtainedTheItem
	waitmessage
	waitfanfare
	additem ITEM_POTION
	msgbox Route1_Text_PutPotionAway
	call EventScript_RestorePrevTextColor
	setflag FLAG_GOT_POTION_ON_ROUTE_1
	release
	end

Route1_EventScript_AlreadyGotPotion::
	msgbox Route1_Text_ComeSeeUsIfYouNeedPokeBalls
	release
	end

Route1_EventScript_Boy::
	msgbox Route1_Text_CanJumpFromLedges, MSGBOX_NPC
	end

Route1_EventScript_RouteSign::
	msgbox Route1_Text_RouteSign, MSGBOX_SIGN
	end

Route1_Text_WorkAtPokeMartTakeSample::
	.string "Hi!\n"
	.string "I work at a POKéMON MART.\p"
	.string "It's part of a convenient chain\n"
	.string "selling all sorts of items.\p"
	.string "Please, visit us in VIRIDIAN CITY.\p"
	.string "I know, I'll give you a sample.\n"
	.string "Here you go!$"

Route1_Text_ComeSeeUsIfYouNeedPokeBalls::
	.string "Please come see us if you need\n"
	.string "POKé BALLS for catching POKéMON.$"

Route1_Text_PutPotionAway::
	.string "{PLAYER} put the POTION away in\n"
	.string "the BAG's ITEMS POCKET.$"

Route1_Text_CanJumpFromLedges::
	.string "See those ledges along the road?\p"
	.string "It's a bit scary, but you can jump\n"
	.string "from them.\p"
	.string "You can get back to PALLET TOWN\n"
	.string "quicker that way.$"

Route1_Text_RouteSign::
	.string "ROUTE 1\n"
	.string "PALLET TOWN - VIRIDIAN CITY$"

