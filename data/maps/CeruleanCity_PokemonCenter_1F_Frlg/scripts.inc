@ Golden Yellow Route 24 Charmander rescue: FLAG_0x0BD hides the temporary
@ treatment-scene Charmander. VAR_0x40FC state 1 is TREATMENT_PENDING.

CeruleanCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CeruleanCity_PokemonCenter_1F_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

CeruleanCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_CERULEAN_CITY
	call_if_eq VAR_0x40FC, 1, CeruleanCity_PokemonCenter_1F_EventScript_ShowRescueCharmander
	call_if_ne VAR_0x40FC, 1, CeruleanCity_PokemonCenter_1F_EventScript_HideRescueCharmander
	end

CeruleanCity_PokemonCenter_1F_EventScript_ShowRescueCharmander::
	clearflag FLAG_0x0BD
	return

CeruleanCity_PokemonCenter_1F_EventScript_HideRescueCharmander::
	setflag FLAG_0x0BD
	return

CeruleanCity_PokemonCenter_1F_OnFrame::
	map_script_2 VAR_0x40FC, 1, CeruleanCity_PokemonCenter_1F_EventScript_CharmanderTreatment
	.2byte 0

CeruleanCity_PokemonCenter_1F_EventScript_CharmanderTreatment::
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	callnative GoldenYellowCharmander_EnsurePartnerVisible, requests_effects=1
	lockall
	setobjectxy LOCALID_PLAYER, 7, 4
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 7, 5
	setobjectxy LOCALID_CERULEAN_RESCUE_CHARMANDER, 8, 4
	applymovement LOCALID_PLAYER, CeruleanCity_PokemonCenter_1F_Movement_PlayerFaceRight
	applymovement OBJ_EVENT_ID_FOLLOWER, CeruleanCity_PokemonCenter_1F_Movement_PartnerFaceUp
	applymovement LOCALID_CERULEAN_RESCUE_CHARMANDER, CeruleanCity_PokemonCenter_1F_Movement_CharmanderFaceLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox CeruleanCity_PokemonCenter_1F_Text_NurseSeesCharmander
	closemessage
	applymovement LOCALID_CERULEAN_RESCUE_CHARMANDER, CeruleanCity_PokemonCenter_1F_Movement_CharmanderFaceUp
	waitmovement LOCALID_CERULEAN_RESCUE_CHARMANDER
	playse SE_BALL
	waitse
	removeobject LOCALID_CERULEAN_RESCUE_CHARMANDER
	applymovement LOCALID_CERULEAN_NURSE, Common_Movement_WalkInPlaceFasterLeft
	waitmovement LOCALID_CERULEAN_NURSE
	callnative GoldenYellowCharmander_StartSingleHeal, requests_effects=1
	gotonative GoldenYellowCharmander_WaitSingleHeal
	applymovement LOCALID_CERULEAN_NURSE, Common_Movement_WalkInPlaceFasterDown
	waitmovement LOCALID_CERULEAN_NURSE
	addobject LOCALID_CERULEAN_RESCUE_CHARMANDER
	setobjectxy LOCALID_CERULEAN_RESCUE_CHARMANDER, 8, 4
	applymovement LOCALID_CERULEAN_RESCUE_CHARMANDER, CeruleanCity_PokemonCenter_1F_Movement_CharmanderFaceLeft
	waitmovement LOCALID_CERULEAN_RESCUE_CHARMANDER
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	msgbox CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderRecovered
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CeruleanCity_PokemonCenter_1F_Text_CharmanderFeelsBetter
	closemessage
	applymovement LOCALID_PLAYER, CeruleanCity_PokemonCenter_1F_Movement_PlayerClearExit
	applymovement OBJ_EVENT_ID_FOLLOWER, CeruleanCity_PokemonCenter_1F_Movement_PartnerClearExit
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderBolts
	closemessage
	applymovement LOCALID_CERULEAN_RESCUE_CHARMANDER, CeruleanCity_PokemonCenter_1F_Movement_CharmanderRunsOut
	waitmovement LOCALID_CERULEAN_RESCUE_CHARMANDER
	setflag FLAG_0x0BD
	removeobject LOCALID_CERULEAN_RESCUE_CHARMANDER
	setvar VAR_0x40FC, 2
	msgbox CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderReturnedRoute24
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

CeruleanCity_PokemonCenter_1F_EventScript_RescueCharmander::
	lock
	faceplayer
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	msgbox CeruleanCity_PokemonCenter_1F_Text_CharmanderBeingTreated
	release
	end

CeruleanCity_PokemonCenter_1F_Movement_PlayerFaceRight::
	face_right
	step_end

CeruleanCity_PokemonCenter_1F_Movement_PartnerFaceUp::
	face_up
	step_end

CeruleanCity_PokemonCenter_1F_Movement_CharmanderFaceLeft::
	face_left
	step_end

CeruleanCity_PokemonCenter_1F_Movement_CharmanderFaceUp::
	face_up
	step_end

CeruleanCity_PokemonCenter_1F_Movement_PlayerClearExit::
	walk_left
	walk_in_place_faster_right
	step_end

CeruleanCity_PokemonCenter_1F_Movement_PartnerClearExit::
	walk_left
	walk_in_place_faster_up
	step_end

CeruleanCity_PokemonCenter_1F_Movement_CharmanderRunsOut::
	walk_fast_left
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

CeruleanCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

CeruleanCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox CeruleanCity_PokemonCenter_1F_Text_EveryoneCallsBillPokemaniac, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_EventScript_Rocker::
	msgbox CeruleanCity_PokemonCenter_1F_Text_BillDoesWhateverForRareMons, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_EventScript_Youngster::
	lock
	faceplayer
	famechecker FAMECHECKER_BILL, 1
	msgbox CeruleanCity_PokemonCenter_1F_Text_BillCollectsRareMons
	release
	end

CeruleanCity_PokemonCenter_1F_EventScript_Lass::
	msgbox CeruleanCity_PokemonCenter_1F_Text_TryTradingUpstairs, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_Text_NurseSeesCharmander::
	.string "Oh! This CHARMANDER is exhausted.\p"
	.string "You did the right thing bringing it\n"
	.string "here. Let me take care of it.$"

CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderRecovered::
	.string "There. CHARMANDER has recovered.\p"
	.string "Its flame is strong again.$"

CeruleanCity_PokemonCenter_1F_Text_CharmanderFeelsBetter::
	.string "CHARMANDER looks relieved.\p"
	.string "But it keeps looking toward the\n"
	.string "door...$"

CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderBolts::
	.string "NURSE JOY: CHARMANDER? Wait!$"

CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderReturnedRoute24::
	.string "It ran out toward ROUTE 24!\p"
	.string "Maybe it's still waiting for its\n"
	.string "TRAINER.$"

CeruleanCity_PokemonCenter_1F_Text_CharmanderBeingTreated::
	.string "CHARMANDER is being treated.$"

CeruleanCity_PokemonCenter_1F_Text_BillDoesWhateverForRareMons::
	.string "That BILL!\p"
	.string "I heard that he'll do whatever it\n"
	.string "takes to get rare POKéMON.\p"
	.string "He's not above doing all sorts\n"
	.string "of things, I've heard.$"

CeruleanCity_PokemonCenter_1F_Text_EveryoneCallsBillPokemaniac::
	.string "Have you heard about BILL?\p"
	.string "Everyone calls him a POKéMANIAC!\p"
	.string "I think people are just jealous of\n"
	.string "BILL, though.\p"
	.string "Who wouldn't want to boast about\n"
	.string "their POKéMON?$"

CeruleanCity_PokemonCenter_1F_Text_BillCollectsRareMons::
	.string "BILL has lots of POKéMON!\n"
	.string "He collects rare ones, too!$"

CeruleanCity_PokemonCenter_1F_Text_TryTradingUpstairs::
	.string "Why don't you go upstairs and try\n"
	.string "trading POKéMON with your friends?\p"
	.string "You could get a lot more variety\n"
	.string "by trading.\p"
	.string "The POKéMON you get in trades grow\n"
	.string "quickly, too.$"

