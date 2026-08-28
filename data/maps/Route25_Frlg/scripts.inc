.equ YELLOW_BILL_DOOR_SPAWN_CHECK, VAR_TEMP_4

Route25_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route25_OnFrame
	.byte 0

Route25_OnFrame::
	map_script_2 YELLOW_BILL_DOOR_SPAWN_CHECK, 0, Route25_EventScript_CheckBillDoorSpawn
	.2byte 0

@ The Before Bill checkpoint intentionally lands on (51,5), directly outside
@ Sea Cottage. Run one load-time coordinate check so that checkpoint testing
@ gets the same pre-door Partner handling as a player who naturally walks onto
@ the approach tile.
Route25_EventScript_CheckBillDoorSpawn::
	setvar YELLOW_BILL_DOOR_SPAWN_CHECK, 1
	special StorePlayerCoordsInVars
	goto_if_ne VAR_0x8004, 51, Route25_EventScript_CheckBillDoorSpawnReturn
	goto_if_ne VAR_0x8005, 5, Route25_EventScript_CheckBillDoorSpawnReturn
	goto Route25_EventScript_PrepareBillDoorPartner

Route25_EventScript_CheckBillDoorSpawnReturn::
	end

@ Yellow's Partner does not return to its Poké Ball at Bill's doorway. The
@ follower is a reserved runtime object, not a Route 25 object template, so use
@ the Golden Yellow follower helper instead of setobjectmovementtype.
Route25_EventScript_PrepareBillDoorPartner::
	goto_if_set FLAG_HELPED_BILL_IN_SEA_COTTAGE, Route25_EventScript_CheckBillDoorSpawnReturn
	goto_if_unset FLAG_PARTNER_PIKACHU_FOLLOWING, Route25_EventScript_CheckBillDoorSpawnReturn
	callnative GoldenYellowPartner_HoldAtBillDoor
	end

@ If Red steps away instead of entering Sea Cottage, immediately return Partner
@ Pikachu to normal following so this narrow doorway guard cannot leak outward.
Route25_EventScript_RestoreBillDoorPartner::
	goto_if_unset FLAG_PARTNER_PIKACHU_FOLLOWING, Route25_EventScript_RestoreBillDoorPartnerReturn
	callnative GoldenYellowPartner_ResumeFollowing

Route25_EventScript_RestoreBillDoorPartnerReturn::
	end

Route25_EventScript_SeaCottageSign::
	msgbox Route25_Text_SeaCottageSign, MSGBOX_SIGN
	end

Route25_EventScript_Man::
	msgbox Route25_Text_AreYouHereAlone, MSGBOX_NPC
	end

Route25_Text_JoeyIntro::
	.string "Local TRAINERS come here to\n"
	.string "practice.$"

Route25_Text_JoeyDefeat::
	.string "You're decent.$"

Route25_Text_JoeyPostBattle::
	.string "All POKéMON have weaknesses.\n"
	.string "Even the strongest ones.\p"
	.string "That's why it's best to raise\n"
	.string "POKéMON of different types.$"

Route25_Text_DanIntro::
	.string "Dad took me to a great party on\n"
	.string "the S.S. ANNE at VERMILION CITY.$"

Route25_Text_DanDefeat::
	.string "I'm not mad!$"

Route25_Text_DanPostBattle::
	.string "On the S.S. ANNE, I saw TRAINERS\n"
	.string "from around the world.$"

Route25_Text_FlintIntro::
	.string "I'm a cool guy.\n"
	.string "I've got a girlfriend!$"

Route25_Text_FlintDefeat::
	.string "Aww, darn…$"

Route25_Text_FlintPostBattle::
	.string "Oh, well.\n"
	.string "My girlfriend will cheer me up.$"

Route25_Text_KelseyIntro::
	.string "Hi!\n"
	.string "My boyfriend is cool!$"

Route25_Text_KelseyDefeat::
	.string "My conditioning isn't the best…$"

Route25_Text_KelseyPostBattle::
	.string "I wish my boyfriend was as good\n"
	.string "as you.$"

Route25_Text_ChadIntro::
	.string "I had this feeling…\n"
	.string "I knew I had to battle you!$"

Route25_Text_ChadDefeat::
	.string "I knew I'd lose, too!$"

Route25_Text_ChadPostBattle::
	.string "If your POKéMON gets confused,\n"
	.string "switch it out.\p"
	.string "That's a good tactic.$"

Route25_Text_HaleyIntro::
	.string "My friend has many cute POKéMON.\n"
	.string "I'm so jealous!$"

Route25_Text_HaleyDefeat::
	.string "I'm not so jealous!$"

Route25_Text_HaleyPostBattle::
	.string "You came from MT. MOON?\n"
	.string "May I have a CLEFAIRY?$"

Route25_Text_FranklinIntro::
	.string "I just got down from MT. MOON,\n"
	.string "but I've still got gas in the tank!$"

Route25_Text_FranklinDefeat::
	.string "You worked hard!$"

Route25_Text_FranklinPostBattle::
	.string "Drat!\n"
	.string "A ZUBAT bit me back in that cave.$"

Route25_Text_NobIntro::
	.string "I'm off to see a POKéMANIAC's\n"
	.string "collection at the cape.$"

Route25_Text_NobDefeat::
	.string "You done got me, and real good,\n"
	.string "too!$"

Route25_Text_NobPostBattle::
	.string "The POKéMANIAC sure lives up to\n"
	.string "his name.\p"
	.string "His collection includes many rare\n"
	.string "species of POKéMON.$"

Route25_Text_WayneIntro::
	.string "You're going to see BILL?\n"
	.string "First, we battle!$"

Route25_Text_WayneDefeat::
	.string "You're something.$"

Route25_Text_WaynePostBattle::
	.string "The trail below is a shortcut to\n"
	.string "CERULEAN CITY.$"

Route25_Text_SeaCottageSign::
	.string "SEA COTTAGE\n"
	.string "BILL lives here!$"

Route25_Text_MistyHighHopesAboutThisPlace::
	.string "This cape is a famous date spot.\p"
	.string "MISTY, the GYM LEADER, has high\n"
	.string "hopes about this place.$"

Route25_Text_AreYouHereAlone::
	.string "Hello, are you here alone?\p"
	.string "If you're out at CERULEAN's cape…\n"
	.string "Well, it should be as a couple.$"

