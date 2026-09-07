CeruleanCity_House3_Frlg_MapScripts::
	.byte 0

CeruleanCity_House3_EventScript_OldWoman::
	msgbox CeruleanCity_House3_Text_PleaseTradeWithMyHusband, MSGBOX_NPC
	end

CeruleanCity_House3_EventScript_Dontae::
	lock
	faceplayer
	setvar VAR_0x8008, INGAME_TRADE_JYNX
	call EventScript_GetInGameTradeSpeciesInfo
	goto_if_set FLAG_DID_ZYNX_TRADE, CeruleanCity_House3_EventScript_AlreadyTraded
	msgbox Trade_Text_DoYouHaveMonWouldYouTradeForMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeruleanCity_House3_EventScript_DeclineTrade
	call EventScript_ChooseMonForInGameTrade
	goto_if_ge VAR_0x8004, PARTY_SIZE, CeruleanCity_House3_EventScript_DeclineTrade
	call EventScript_GetInGameTradeSpecies
	goto_if_ne VAR_RESULT, VAR_0x8009, CeruleanCity_House3_EventScript_NotRequestedMon
	call EventScript_DoInGameTrade
	msgbox Trade_Text_Thanks
	setflag FLAG_DID_ZYNX_TRADE
	release
	end

CeruleanCity_House3_EventScript_DeclineTrade::
	msgbox Trade_Text_WellIfYouDontWantTo
	release
	end

CeruleanCity_House3_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_ThisIsntMon
	release
	end

CeruleanCity_House3_EventScript_AlreadyTraded::
	msgbox Trade_Text_HasTradedMonGrownStronger
	release
	end

CeruleanCity_House3_Text_PleaseTradeWithMyHusband::
	.string "My husband likes trading POKéMON.\p"
	.string "You're collecting POKéMON for your\n"
	.string "POKéDEX, aren't you?\p"
	.string "Would you please trade with him?$"

.equ GY_MELANIE_NO_PARTNER,      0
.equ GY_MELANIE_FRIENDSHIP_LOW,  1
.equ GY_MELANIE_ELIGIBLE,        2

CeruleanCity_House3_EventScript_Melanie::
	lock
	faceplayer
	goto_if_set FLAG_HIDE_CERULEAN_HOUSE3_BULBASAUR, CeruleanCity_House3_EventScript_MelaniePostGift
	callnative GoldenYellow_CheckMelanieBulbasaurEligibility
	goto_if_eq VAR_RESULT, GY_MELANIE_NO_PARTNER, CeruleanCity_House3_EventScript_MelanieNoPartner
	goto_if_eq VAR_RESULT, GY_MELANIE_FRIENDSHIP_LOW, CeruleanCity_House3_EventScript_MelanieFriendshipLow
	goto CeruleanCity_House3_EventScript_MelanieOffer
	end

CeruleanCity_House3_EventScript_MelanieNoPartner::
	msgbox CeruleanCity_House3_Text_MelanieNoPartner
	release
	end

CeruleanCity_House3_EventScript_MelanieFriendshipLow::
	msgbox CeruleanCity_House3_Text_MelanieFriendshipLow
	release
	end

CeruleanCity_House3_EventScript_MelanieOffer::
	applymovement LOCALID_CERULEAN_HOUSE3_BULBASAUR, Common_Movement_FacePlayer
	waitmovement LOCALID_CERULEAN_HOUSE3_BULBASAUR
	msgbox CeruleanCity_House3_Text_MelanieRecognizesTrust
	playmoncry SPECIES_BULBASAUR, CRY_MODE_NORMAL
	waitmoncry
	msgbox CeruleanCity_House3_Text_MelanieOfferBulbasaur, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeruleanCity_House3_EventScript_MelanieDeclined
	setvar VAR_TEMP_1, SPECIES_BULBASAUR
	givemon SPECIES_BULBASAUR, 10
	goto_if_eq VAR_RESULT, 0, CeruleanCity_House3_EventScript_ReceiveBulbasaurParty
	goto_if_eq VAR_RESULT, 1, CeruleanCity_House3_EventScript_ReceiveBulbasaurPC
	goto_if_eq VAR_RESULT, 2, CeruleanCity_House3_EventScript_BulbasaurNoStorage
	release
	end

CeruleanCity_House3_EventScript_MelanieDeclined::
	msgbox CeruleanCity_House3_Text_MelanieDeclined
	applymovement LOCALID_CERULEAN_HOUSE3_BULBASAUR, Common_Movement_FaceDown
	waitmovement LOCALID_CERULEAN_HOUSE3_BULBASAUR
	release
	end

CeruleanCity_House3_EventScript_BulbasaurNoStorage::
	applymovement LOCALID_CERULEAN_HOUSE3_BULBASAUR, Common_Movement_FaceDown
	waitmovement LOCALID_CERULEAN_HOUSE3_BULBASAUR
	goto Common_EventScript_NoMoreRoomForPokemon
	end

CeruleanCity_House3_EventScript_CommitBulbasaurGift::
	setflag FLAG_HIDE_CERULEAN_HOUSE3_BULBASAUR
	removeobject LOCALID_CERULEAN_HOUSE3_BULBASAUR
	return

CeruleanCity_House3_EventScript_ReceiveBulbasaurParty::
	call CeruleanCity_House3_EventScript_CommitBulbasaurGift
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message CeruleanCity_House3_Text_ObtainedBulbasaur
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeruleanCity_House3_EventScript_FinishBulbasaurGift
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto CeruleanCity_House3_EventScript_FinishBulbasaurGift
	end

CeruleanCity_House3_EventScript_ReceiveBulbasaurPC::
	call CeruleanCity_House3_EventScript_CommitBulbasaurGift
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message CeruleanCity_House3_Text_ObtainedBulbasaur
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeruleanCity_House3_EventScript_BulbasaurTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto CeruleanCity_House3_EventScript_BulbasaurTransferredToPC
	end

CeruleanCity_House3_EventScript_BulbasaurTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto CeruleanCity_House3_EventScript_FinishBulbasaurGift
	end

CeruleanCity_House3_EventScript_FinishBulbasaurGift::
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox CeruleanCity_House3_Text_MelanieAfterGift
	callnative GoldenYellow_StartMelanieBulbasaurPartnerReaction, requests_effects=1
	release
	end

CeruleanCity_House3_EventScript_MelaniePostGift::
	msgbox CeruleanCity_House3_Text_MelaniePostGift
	release
	end

CeruleanCity_House3_EventScript_Bulbasaur::
	lock
	faceplayer
	playmoncry SPECIES_BULBASAUR, CRY_MODE_NORMAL
	waitmoncry
	msgbox CeruleanCity_House3_Text_BulbasaurWatchesPlayer
	applymovement LOCALID_CERULEAN_HOUSE3_BULBASAUR, Common_Movement_FaceDown
	waitmovement LOCALID_CERULEAN_HOUSE3_BULBASAUR
	release
	end

CeruleanCity_House3_Text_MelanieNoPartner::
	.string "I care for POKéMON that have been\n"
	.string "abandoned or mistreated.\p"
	.string "I need to see the trust between you\n"
	.string "and your special PIKACHU first.$"

CeruleanCity_House3_Text_MelanieFriendshipLow::
	.string "This BULBASAUR needs a TRAINER it\n"
	.string "can trust completely.\p"
	.string "Spend more time with PIKACHU and\n"
	.string "show me how close you've become.$"

CeruleanCity_House3_Text_MelanieRecognizesTrust::
	.string "Your PIKACHU trusts you deeply.\p"
	.string "I can see that you treat POKéMON\n"
	.string "with real kindness.$"

CeruleanCity_House3_Text_MelanieOfferBulbasaur::
	.string "This BULBASAUR has suffered, but it\n"
	.string "is ready to trust again.\p"
	.string "Will you take care of BULBASAUR?$"

CeruleanCity_House3_Text_MelanieDeclined::
	.string "That's all right. BULBASAUR will\n"
	.string "wait until you're ready.$"

CeruleanCity_House3_Text_ObtainedBulbasaur::
	.string "{PLAYER} received a BULBASAUR!$"

CeruleanCity_House3_Text_MelanieAfterGift::
	.string "Please take good care of BULBASAUR!\p"
	.string "PIKACHU looks happy to welcome a\n"
	.string "new friend.$"

CeruleanCity_House3_Text_MelaniePostGift::
	.string "How is BULBASAUR doing?\p"
	.string "With you and PIKACHU, I know it is\n"
	.string "in good hands.$"

CeruleanCity_House3_Text_BulbasaurWatchesPlayer::
	.string "BULBASAUR watches you cautiously...\p"
	.string "It seems interested in you.$"
