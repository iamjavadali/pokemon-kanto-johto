from pathlib import Path

path = Path("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
text = path.read_text()

old = """PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitRight::
\tapplymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerStepLeftWatchRival
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalExitStraight
\twaitmovement LOCALID_OAKS_LAB_RIVAL
\tgoto PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit
\tend

PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit::
"""
new = """PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitRight::
\tapplymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerStepLeftWatchRival
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalExitRight
\twaitmovement LOCALID_OAKS_LAB_RIVAL
\tgoto PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit
\tend

PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit::
"""
if text.count(old) != 1:
    raise SystemExit(f"right exit script guard failed: expected 1 match, found {text.count(old)}")
text = text.replace(old, new, 1)

anchor = """PalletTown_ProfessorOaksLab_Movement_YellowRivalExitStraight::
\twalk_down
\twalk_down
\twalk_down
\twalk_down
\twalk_down
\tstep_end

"""
addition = anchor + """PalletTown_ProfessorOaksLab_Movement_YellowRivalExitRight::
\twalk_left
\twalk_left
\twalk_down
\twalk_down
\twalk_down
\twalk_down
\twalk_down
\tstep_end

"""
if text.count(anchor) != 1:
    raise SystemExit(f"right exit movement guard failed: expected 1 anchor, found {text.count(anchor)}")
text = text.replace(anchor, addition, 1)
path.write_text(text)
print("Right-side rival exit now routes through the x=5 lab doorway.")
