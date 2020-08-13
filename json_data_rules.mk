# JSON files are run through jsonproc, which is a tool that converts JSON data to an output file
# based on an Inja template. https://github.com/pantor/inja


$(DATA_SRC_SUBDIR)/wild_encounters.h: $(DATA_SRC_SUBDIR)/wild_encounters.json $(DATA_SRC_SUBDIR)/wild_encounters.json.txt
	$(JSONPROC) $^ $@

$(C_BUILDDIR)/wild_encounter.o: c_dep += $(DATA_SRC_SUBDIR)/wild_encounters.h


TRAINERS_JSON := data/trainers.json

TRAINER_HEADERS :=                   \
    include/constants/opponents.h    \
    src/data/trainers.h              \
    src/data/trainer_parties.h

$(TRAINER_HEADERS): %.h: $(TRAINERS_JSON) %.h.template
	$(JSONPROC) $^ $@

trainers_templates: $(TRAINER_HEADERS)


AUTO_GEN_HEADERS += $(DATA_SRC_SUBDIR)/wild_encounters.h $(TRAINER_HEADERS)
