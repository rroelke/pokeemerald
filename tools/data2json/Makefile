CXX ?= g++

CXX_DEBUG_FLAGS := -g -O0
CXX_NDEBUG_FLAGS := -g -O2

DEBUG := 1

ifeq "x$(DEBUG)" "x"
CXXFLAGS += -Wall -std=c++11 $(CXX_NDEBUG_FLAGS)
else
CXXFLAGS += -Wall -std=c++11 $(CXX_DEBUG_FLAGS)
endif

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif


EMERALD_DIR := $(realpath ../../)
BUILD_DIR := build

EXECS := $(addsuffix $(EXE), species2json trainers2json)


IPATH := -iquote $(EMERALD_DIR)/include -iquote $(EMERALD_DIR)/src -iquote $(EMERALD_DIR)/gflib

CPP_INCLUDES := nlohmann/json.hpp build/ability_names.h build/item_names.h build/move_names.h build/trainer_srcid.h build/trainer_classes.h build/trainer_pics.h build/type_names.h text.h
C_INCLUDES := build/encounter_music.h build/species_srcid.h

all: $(EXECS)

C_OBJS := $(addprefix $(BUILD_DIR)/, $(addsuffix .o, base_stats evolution graphics level_up_learnsets move_names move_tutor pokedex_entries text tmhm trainers))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/ability_names.h: $(EMERALD_DIR)/include/constants/abilities.h $(BUILD_DIR)
	echo "const char *const abilityNames[] = {" > $@
	cat $< | grep "#define ABILITY_" | sed 's/^#define ABILITY_\([A-Za-z_0-9]*\)\( *\)\([0-9]*\)$$/    [ABILITY_\1] = "\1",/g' >> $@
	echo "};" >> $@
$(BUILD_DIR)/item_names.h: $(EMERALD_DIR)/include/constants/items.h $(BUILD_DIR)
	echo "const char *const itemNames[] = {" > $@
	cat $< | grep "#define ITEM_" | sed -n 's/^#define ITEM_\([A-Za-z_0-9]*\)\( *\)\([0-9]*\)$$/    [ITEM_\1] = "\1",/p' >> $@
	echo "};" >> $@
$(BUILD_DIR)/move_names.h: $(EMERALD_DIR)/include/constants/moves.h $(BUILD_DIR)
	echo "const char *const moveNames[] = {" > $@
	cat $< | grep "#define MOVE_" | sed 's/#define MOVE_\([A-Z_2]*\)\( *\)\([0-9]*\)/    [MOVE_\1] = "\1",/g' >> $@
	echo "};" >> $@
$(BUILD_DIR)/species_srcid.h: $(EMERALD_DIR)/include/constants/species.h $(BUILD_DIR)
	echo "const char *const speciesSourceNames[] = {" > $@
	cat $< | grep "#define SPECIES_" | sed 's/#define SPECIES_\([A-Za-z_2]*\)\( *\)\([0-9]*\)/    [SPECIES_\1] = "\1",/g' >> $@
	echo "};" >> $@
$(BUILD_DIR)/trainer_srcid.h: $(EMERALD_DIR)/include/constants/opponents.h $(BUILD_DIR)
	echo "const char *const trainerSourceNames[] = {" > $@
	cat $< | grep "#define TRAINER_" | sed 's/#define TRAINER_\([A-Z_0-9]*\)\( *\)\([0-9]*\)/    [TRAINER_\1] = "\1",/g' >> $@
	echo "};" >> $@
$(BUILD_DIR)/trainer_classes.h: $(EMERALD_DIR)/include/constants/trainers.h $(BUILD_DIR)
	echo "const char *const trainerClassNames[] = {" > $@
	cat $< | grep "#define TRAINER_CLASS_" | sed 's/#define TRAINER_CLASS_\([A-Z_0-9]*\)\( *\)0x\([0-9A-Fa-f]*\)/    [TRAINER_CLASS_\1] = "\1",/g' >> $@
	echo "};" >> $@
$(BUILD_DIR)/trainer_pics.h: $(EMERALD_DIR)/include/constants/trainers.h $(BUILD_DIR)
	echo "const char *const trainerPicNames[] = {" > $@
	cat $< | grep "#define TRAINER_PIC_" | sed 's/#define TRAINER_PIC_\([A-Z_0-9]*\)\( *\)\([0-9]*\)/    [TRAINER_PIC_\1] = "\1",/g' >> $@
	echo "};" >> $@
$(BUILD_DIR)/type_names.h: $(EMERALD_DIR)/include/constants/pokemon.h $(BUILD_DIR)
	echo "const char *const typeNames[] = {" > $@
	cat $< | grep "#define TYPE_" | grep -v TYPE_NONE | sed 's/#define TYPE_\([A-Z]*\)\( *\)\([0-9]*\)/    [TYPE_\1] = "\1",/g' >> $@
	echo "};" >> $@
$(BUILD_DIR)/encounter_music.h: $(EMERALD_DIR)/include/constants/trainers.h $(BUILD_DIR)
	echo "const char *const encounterMusicSourceNames[] = {" > $@
	cat $^ | grep TRAINER_ENCOUNTER_MUSIC_ | sed 's/#define TRAINER_ENCOUNTER_MUSIC_\([A-Z_]*\)\( *\)\([0-9]*\)/    [TRAINER_ENCOUNTER_MUSIC_\1] = "\1",/g' >> $@
	echo "};" >> $@

$(C_OBJS): $(BUILD_DIR)/%.o: %.c $(BUILD_DIR) $(C_INCLUDES)
	$(CC) $(CFLAGS) $(IPATH) $< -c -o $@

$(EXECS): %: %.cpp $(C_OBJS) $(CPP_INCLUDES)
	$(CXX) $(CXXFLAGS) $(IPATH) $< $(C_OBJS) -o $@ $(LDFLAGS)

.PHONY: clean 
clean:
	$(RM) -rf $(EXECS) build
