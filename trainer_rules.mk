# party files are run through trainerproc, which is a tool that converts party data to an output file
# matching the current trainer .h formatting

AUTO_GEN_TARGETS += src/data/trainers.h
AUTO_GEN_TARGETS += src/data/trainers_frlg.h
AUTO_GEN_TARGETS += src/data/battle_partners.h
AUTO_GEN_TARGETS += test/battle/trainer_control.h
AUTO_GEN_TARGETS += test/battle/partner_control.h
AUTO_GEN_TARGETS += src/data/debug_trainers.h

YELLOW_RIVAL_PARTY_FILTER := tools/golden_yellow/apply_yellow_rival_parties.py

# Golden Yellow keeps the upstream-compatible FRLG party source and overlays only
# the Stage 1 rival rosters before trainerproc. This preserves trainer numeric IDs
# and all non-rival trainer data while making the Yellow roster definitions the
# generated source of truth for the prototype.
src/data/trainers_frlg.h: src/data/trainers_frlg.party $(YELLOW_RIVAL_PARTY_FILTER) $(TRAINERPROC)
	python3 $(YELLOW_RIVAL_PARTY_FILTER) $< | $(CPP) $(CPPFLAGS) -traditional-cpp - | $(TRAINERPROC) -o $@ -i $< -

%.h: %.party $(TRAINERPROC)
	$(CPP) $(CPPFLAGS) -traditional-cpp - < $< | $(TRAINERPROC) -o $@ -i $< -
