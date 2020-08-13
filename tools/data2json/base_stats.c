#include "gba/defines.h"
#include "gba/types.h"

#include "constants/abilities.h"
#include "constants/global.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#include "structs.h"

#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#include "data/pokemon/base_stats.h"

#define _(string) string
#include "data/text/abilities.h"
