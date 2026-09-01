.set SPOKE_TO_WOMAN_LAST,   FLAG_TEMP_2
.set SPOKE_TO_FAT_MAN_LAST, FLAG_TEMP_3
.set YELLOW_FAN_CLUB_ENTRY_PENDING, VAR_TEMP_4
.set YELLOW_FAN_CLUB_PARTNER_STATE, VAR_UNUSED_0x40FE
.set YELLOW_FAN_CLUB_PARTNER_PARKED, 3

VermilionCity_PokemonFanClub_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_PokemonFanClub_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VermilionCity_PokemonFanClub_OnFrame
	.byte 0

VermilionCity_PokemonFanClub_OnTransition::
	callnative GoldenYellow_RestoreFanClubPartnerOnFollower
	setvar YELLOW_FAN_CLUB_ENTRY_PENDING, 1
	end

VermilionCity_PokemonFanClub_OnFrame::
	map_script_2 YELLOW_FAN_CLUB_ENTRY_PENDING, 1, VermilionCity_PokemonFanClub_EventScript_PartnerArrival
	.2byte 0

@ Match the proven Bill-house entry lifecycle: arm during transition and consume
@ on the first controllable map frame, before the player can walk into the room.
@ The brief settle delay lets the following-Pokemon object finish map entry;
@ P7C then stages Partner beside the player, shows !, and owns the authored walk.
VermilionCity_PokemonFanClub_EventScript_PartnerArrival::
	lockall
	setvar YELLOW_FAN_CLUB_ENTRY_PENDING, 0
	delay 8
	callnative GoldenYellow_StartFanClubPartnerArrival
	releaseall
	end

VermilionCity_PokemonFanClub_EventScript_PartnerExitCheck::
	lockall
	goto_if_eq YELLOW_FAN_CLUB_PARTNER_STATE, YELLOW_FAN_CLUB_PARTNER_PARKED, VermilionCity_PokemonFanClub_EventScript_BlockExit
	callnative GoldenYellow_TryReleaseFanClubPartnerOnExit
	releaseall
	end

@ Mirror the accepted Pewter Jigglypuff exit guard: while the authored scene owns
@ Partner, the player may not cross a room exit and leave Pikachu behind. Once
@ Emotion 30 returns Partner to FOLLOW_PLAYER, the saved state clears and this
@ same exit row falls through to normal room departure.
VermilionCity_PokemonFanClub_EventScript_BlockExit::
	playmoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL
	waitmoncry
	msgbox VermilionCity_PokemonFanClub_Text_PartnerBusyDontLeave, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, VermilionCity_PokemonFanClub_Movement_PlayerBackFromExit
	waitmovement 0
	releaseall
	end

VermilionCity_PokemonFanClub_Movement_PlayerBackFromExit::
	walk_up
	step_end

VermilionCity_PokemonFanClub_EventScript_Chairman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_BIKE_VOUCHER, VermilionCity_PokemonFanClub_EventScript_AlreadyHeardStory
	msgbox VermilionCity_PokemonFanClub_Text_DidYouComeToHearAboutMyMons, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, VermilionCity_PokemonFanClub_EventScript_ChairmanStory
	msgbox VermilionCity_PokemonFanClub_Text_ComeBackToHearStory
	release
	end

VermilionCity_PokemonFanClub_EventScript_AlreadyHeardStory::
	msgbox VermilionCity_PokemonFanClub_Text_DidntComeToSeeAboutMonsAgain
	release
	end

VermilionCity_PokemonFanClub_EventScript_ChairmanStory::
	msgbox VermilionCity_PokemonFanClub_Text_ChairmansStory
	checkitemspace ITEM_BIKE_VOUCHER
	goto_if_eq VAR_RESULT, FALSE, VermilionCity_PokemonFanClub_EventScript_NoRoomForBikeVoucher
	setflag FLAG_GOT_BIKE_VOUCHER
	giveitem_msg VermilionCity_PokemonFanClub_Text_ReceivedBikeVoucherFromChairman, ITEM_BIKE_VOUCHER, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox VermilionCity_PokemonFanClub_Text_ExplainBikeVoucher
	release
	end

VermilionCity_PokemonFanClub_EventScript_NoRoomForBikeVoucher::
	msgbox VermilionCity_PokemonFanClub_Text_MakeRoomForThis
	release
	end

VermilionCity_PokemonFanClub_EventScript_WorkerF::
	lock
	faceplayer
	goto_if_set FLAG_SYS_GAME_CLEAR, VermilionCity_PokemonFanClub_EventScript_WorkerFGameClear
	msgbox VermilionCity_PokemonFanClub_Text_ChairmanVeryVocalAboutPokemon
	release
	end

VermilionCity_PokemonFanClub_EventScript_WorkerFGameClear::
	famechecker FAMECHECKER_DAISY, 1
	msgbox VermilionCity_PokemonFanClub_Text_ChairmanReallyAdoresHisMons
	release
	end

VermilionCity_PokemonFanClub_EventScript_Woman::
	lock
	faceplayer
	goto_if_set SPOKE_TO_FAT_MAN_LAST, VermilionCity_PokemonFanClub_EventScript_WomanSpokeToFatMan
	msgbox VermilionCity_PokemonFanClub_Text_AdoreMySeel
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag SPOKE_TO_WOMAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_WomanSpokeToFatMan::
	msgbox VermilionCity_PokemonFanClub_Text_SeelFarMoreAttractive
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	clearflag SPOKE_TO_FAT_MAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_FatMan::
	lock
	faceplayer
	goto_if_set SPOKE_TO_WOMAN_LAST, VermilionCity_PokemonFanClub_EventScript_FatManSpokeToWoman
	msgbox VermilionCity_PokemonFanClub_Text_AdmirePikachusTail
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag SPOKE_TO_FAT_MAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_FatManSpokeToWoman::
	msgbox VermilionCity_PokemonFanClub_Text_PikachuTwiceAsCute
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	clearflag SPOKE_TO_WOMAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_Pikachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox VermilionCity_PokemonFanClub_Text_Pikachu
	waitmoncry
	release
	end

VermilionCity_PokemonFanClub_EventScript_Seel::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SEEL, CRY_MODE_NORMAL
	msgbox VermilionCity_PokemonFanClub_Text_Seel
	waitmoncry
	release
	end

VermilionCity_PokemonFanClub_EventScript_RulesSign1::
	msgbox VermilionCity_PokemonFanClub_Text_ListenPolitelyToOtherTrainers, MSGBOX_SIGN
	end

VermilionCity_PokemonFanClub_EventScript_RulesSign2::
	msgbox VermilionCity_PokemonFanClub_Text_SomeoneBragsBragBack, MSGBOX_SIGN
	end

VermilionCity_PokemonFanClub_Text_AdmirePikachusTail::
	.string "Won't you admire my PIKACHU's\n"
	.string "adorable tail?$"

VermilionCity_PokemonFanClub_Text_PikachuTwiceAsCute::
	.string "Humph!\p"
	.string "My PIKACHU is twice as cute as\n"
	.string "that one!$"

VermilionCity_PokemonFanClub_Text_AdoreMySeel::
	.string "I just adore my SEEL!\n"
	.string "It's so lovable!\p"
	.string "It squeals, “Kyuuuh,” when I\n"
	.string "hug it!$"

VermilionCity_PokemonFanClub_Text_SeelFarMoreAttractive::
	.string "Oh, dear!\p"
	.string "My SEEL is far more attractive.\n"
	.string "By double, I would say.$"

VermilionCity_PokemonFanClub_Text_Pikachu::
	.string "PIKACHU: Chu! Pikachu!$"

VermilionCity_PokemonFanClub_Text_Seel::
	.string "SEEL: Kyuoo!$"

VermilionCity_PokemonFanClub_Text_PartnerBusyDontLeave::
	.string "PIKACHU is still busy with the\n"
	.string "other PIKACHU.\p"
	.string "You shouldn't leave it behind.$"

VermilionCity_PokemonFanClub_Text_DidYouComeToHearAboutMyMons::
	.string "I chair the POKéMON Fan Club!\p"
	.string "I raise more than a hundred\n"
	.string "POKéMON!\p"
	.string "I'm very fussy when it comes to\n"
	.string "POKéMON! I surely am!\p"
	.string "So…\p"
	.string "Did you come visit to hear about\n"
	.string "my POKéMON?$"

VermilionCity_PokemonFanClub_Text_ChairmansStory::
	.string "Good!\n"
	.string "Then listen up!\p"
	.string "My favorite RAPIDASH…\p"
	.string "It's…cute…lovely…smart… \n"
	.string "plus…amazing…you think so?…\l"
	.string "oh yes…it's…stunning…\l"
	.string "kindly…love it!\p"
	.string "Hug it…when…sleeping…warm\n"
	.string "and cuddly…spectacular…\l"
	.string "ravishing…\l"
	.string "…Oops! Look at the time!\l"
	.string "I kept you too long!\p"
	.string "Thanks for hearing me out!\n"
	.string "I want you to have this!$"

VermilionCity_PokemonFanClub_Text_ReceivedBikeVoucherFromChairman::
	.string "{PLAYER} received a BIKE VOUCHER\n"
	.string "from the CHAIRMAN.$"

VermilionCity_PokemonFanClub_Text_ExplainBikeVoucher::
	.string "Take that BIKE VOUCHER to the\n"
	.string "BIKE SHOP in CERULEAN CITY.\p"
	.string "Exchange that for a BICYCLE\n"
	.string "free of charge!\p"
	.string "Don't worry, my favorite FEAROW\n"
	.string "will FLY me anywhere I need to go.\p"
	.string "So, I have no need for a BICYCLE.\p"
	.string "I hope you like cycling!$"

VermilionCity_PokemonFanClub_Text_ComeBackToHearStory::
	.string "Oh. Come back when you want to\n"
	.string "hear my story!$"

VermilionCity_PokemonFanClub_Text_DidntComeToSeeAboutMonsAgain::
	.string "Hello, {PLAYER}!\p"
	.string "Did you come see me about my\n"
	.string "POKéMON again?\p"
	.string "No?\n"
	.string "Too bad!$"

VermilionCity_PokemonFanClub_Text_MakeRoomForThis::
	.string "Make room for this!$"

VermilionCity_PokemonFanClub_Text_ChairmanVeryVocalAboutPokemon::
	.string "Our CHAIRMAN is very vocal about\n"
	.string "POKéMON.$"

VermilionCity_PokemonFanClub_Text_ListenPolitelyToOtherTrainers::
	.string "Let's all listen politely to other\n"
	.string "TRAINERS!$"

VermilionCity_PokemonFanClub_Text_SomeoneBragsBragBack::
	.string "If someone brags, brag right back!$"

VermilionCity_PokemonFanClub_Text_ChairmanReallyAdoresHisMons::
	.string "Our CHAIRMAN really does adore his\n"
	.string "POKéMON.\p"
	.string "But the person who is most liked by\n"
	.string "POKéMON is DAISY, I think.$"
