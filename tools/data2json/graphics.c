#include "gba/types.h"
#include "constants/battle_anim.h"
#include "constants/global.h"
#include "constants/species.h"

struct Coords16 {
    s16 x;
    s16 y;
};

#include "sprite.h"

#include "structs.h"

const union AnimCmd sAnim_GeneralFrame0[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

#include "data/pokemon_graphics/back_animation.h"
#include "data/pokemon_graphics/back_pic_coordinates.h"
#include "data/pokemon_graphics/enemy_mon_elevation.h"
#include "data/pokemon_graphics/front_animation.h"
#include "data/pokemon_graphics/front_pic_anims.h"
#include "data/pokemon_graphics/front_pic_coordinates.h"
#include "data/pokemon_graphics/unknown_anims.h"
