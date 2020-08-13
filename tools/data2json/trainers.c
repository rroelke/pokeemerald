#define _(string) string

#include "gba/defines.h"
#include "gba/types.h"
#include "constants/battle_ai.h"
#include "constants/global.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"

#include "structs.h"

#include "build/encounter_music.h"

#define ARRAY_COUNT(array) (size_t)(sizeof(array) / sizeof((array)[0]))

#include "data/trainer_parties.h"
#include "data/trainers.h"
