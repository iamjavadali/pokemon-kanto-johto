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
	lockall
	setobjectxy LOCALID_PLAYER, 7, 5
	setobjectxy OBJ_EVENT_ID_FOLLOWER, 6, 5
	setobjectxy LOCALID_CERULEAN_RESCUE_CHARMANDER, 8, 4
	applymovement LOCALID_PLAYER, CeruleanCity_PokemonCenter_1F_Movement_PlayerFaceUp
	applymovement OBJ_EVENT_ID_FOLLOWER, CeruleanCity_PokemonCenter_1F_Movement_PartnerFaceRight
	applymovement LOCALID_CERULEAN_RESCUE_CHARMANDER, CeruleanCity_PokemonCenter_1F_Movement_CharmanderFaceLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox CeruleanCity_PokemonCenter_1F_Text_NurseSeesCharmander
	closemessage
	applymovement LOCALID_CERULEAN_NURSE, Common_Movement_WalkInPlaceFasterDown
	waitmovement LOCALID_CERULEAN_NURSE
	dofieldeffect FLDEFF_POKECENTER_HEAL
	waitfieldeffect FLDEFF_POKECENTER_HEAL
	delay 20
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	waitmoncry
	msgbox CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderRecovered
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CeruleanCity_PokemonCenter_1F_Text_CharmanderFeelsBetter
	closemessage
	applymovement LOCALID_CERULEAN_RESCUE_CHARMANDER, CeruleanCity_PokemonCenter_1F_Movement_CharmanderRunsOut
	waitmovement LOCALID_CERULEAN_RESCUE_CHARMANDER
	setflag FLAG_0x0BD
	removeobject LOCALID_CERULEAN_RESCUE_CHARMANDER
	setvar VAR_0x40FC, 2
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderReturnedRoute24
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

CeruleanCity_PokemonCenter_1F_Movement_PlayerFaceUp::
	face_up
	step_end

CeruleanCity_PokemonCenter_1F_Movement_PartnerFaceRight::
	face_right
	step_end

CeruleanCity_PokemonCenter_1F_Movement_CharmanderFaceLeft::
	face_left
	step_end

CeruleanCity_PokemonCenter_1F_Movement_CharmanderRunsOut::
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
	.string "here. Let me treat it.$"

CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderRecovered::
	.string "There. CHARMANDER has recovered.\p"
	.string "Its flame is strong again.$"

CeruleanCity_PokemonCenter_1F_Text_CharmanderFeelsBetter::
	.string "CHARMANDER looks relieved.\p"
	.string "It seems eager to go somewhere.$"

CeruleanCity_PokemonCenter_1F_Text_NurseCharmanderReturnedRoute24::
	.string "It ran out toward ROUTE 24!\p"
	.string "You should make sure it's all right.$"

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

