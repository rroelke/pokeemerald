#include "constants/battle_anim.h"
#include "constants/pokemon.h"

#include <stdlib.h>


extern const char *growthRateToString(int rate) {
    switch (rate) {
        case GROWTH_MEDIUM_FAST:
            return "MEDIUM FAST";
        case GROWTH_ERRATIC:
            return "ERRATIC";
        case GROWTH_FLUCTUATING:
            return "FLUCTUATING";
        case GROWTH_MEDIUM_SLOW:
            return "MEDIUM SLOW";
        case GROWTH_FAST:
            return "FAST";
        case GROWTH_SLOW:
            return "SLOW";
        default:
            return NULL;
    }
}

extern const char *eggGroupToString(int group) {
    switch (group) {
        case EGG_GROUP_MONSTER:
            return "MONSTER";
        case EGG_GROUP_WATER_1:
            return "WATER 1";
        case EGG_GROUP_BUG:
            return "BUG";
        case EGG_GROUP_FLYING:
            return "FLYING";
        case EGG_GROUP_FIELD:
            return "FIELD";
        case EGG_GROUP_FAIRY:
            return "FAIRY";
        case EGG_GROUP_GRASS:
            return "GRASS";
        case EGG_GROUP_HUMAN_LIKE:
            return "HUMAN-LIKE";
        case EGG_GROUP_WATER_3:
            return "WATER 3";
        case EGG_GROUP_MINERAL:
            return "MINERAL";
        case EGG_GROUP_AMORPHOUS:
            return "AMORPHOUS";
        case EGG_GROUP_WATER_2:
            return "WATER 2";
        case EGG_GROUP_DITTO:
            return "DITTO";
        case EGG_GROUP_DRAGON:
            return "DRAGON";
        case EGG_GROUP_UNDISCOVERED:
            return "UNDISCOVERED";
        default:
            return NULL;
    }
}

extern const char *bodyColorToString(int color) {
    switch (color) {
        case BODY_COLOR_RED:
            return "red";
        case BODY_COLOR_BLUE:
            return "blue";
        case BODY_COLOR_YELLOW:
            return "yellow";
        case BODY_COLOR_GREEN:
            return "green";
        case BODY_COLOR_BLACK:
            return "black";
        case BODY_COLOR_BROWN:
            return "brown";
        case BODY_COLOR_PURPLE:
            return "purple";
        case BODY_COLOR_GRAY:
            return "gray";
        case BODY_COLOR_WHITE:
            return "white";
        case BODY_COLOR_PINK:
            return "pink";
        default:
            return NULL;
    }
}

extern const char *backAnimToString(int anim) {
    switch (anim) {
        case BACK_ANIM_H_SLIDE_QUICK:
            return "h slide quick";
        case BACK_ANIM_H_SLIDE:
            return "h slide";
        case BACK_ANIM_H_SLIDE_WITH_V_COMPRESS_1:
            return "h slide with v compress 1";
        case BACK_ANIM_H_SLIDE_WITH_V_COMPRESS_2:
            return "h slide with v compress 2";
        case BACK_ANIM_SHRINK_GROW_1:
            return "shrink grow 1";
        case BACK_ANIM_GROW_1:
            return "grow 1";
        case BACK_ANIM_CIRCLE_MOVE_COUNTERCLOCKWISE:
            return "circle move counterclockwise";
        case BACK_ANIM_HORIZONTAL_SHAKE:
            return "horizontal shake";
        case BACK_ANIM_VERTICAL_SHAKE:
            return "vertical shake";
        case BACK_ANIM_V_SHAKE_WITH_H_SLIDE:
            return "v shake with h slide";
        case BACK_ANIM_VERTICAL_STRETCH:
            return "vertical stretch";
        case BACK_ANIM_HORIZONTAL_STRETCH:
            return "horizontal stretch";
        case BACK_ANIM_GROW_2:
            return "grow 2";
        case BACK_ANIM_V_SHAKE_WITH_PAUSE:
            return "v shake with pause";
        case BACK_ANIM_CIRCLE_MOVE_CLOCKWISE:
            return "circle move clockwise";
        case BACK_ANIM_CONCAVE_DOWN_ARC_SWAY_SMALL:
            return "concave down arc sway small";
        case BACK_ANIM_CONCAVE_DOWN_ARC_SWAY_LARGE:
            return "concave down arc sway large";
        case BACK_ANIM_CONCAVE_UP_ARC_SWAY_LARGE:
            return "concave up arc sway large";
        case BACK_ANIM_DIP_RIGHT_SIDE:
            return "dip right side";
        case BACK_ANIM_SHRINK_GROW_2:
            return "shrink grow 2";
        case BACK_ANIM_JOLT_RIGHT:
            return "jolt right";
        case BACK_ANIM_FLASH_YELLOW_WITH_SHAKE:
            return "flash yellow with shake";
        case BACK_ANIM_FADE_RED_WITH_SHAKE:
            return "fade red with shake";
        case BACK_ANIM_FADE_GREEN_WITH_SHAKE:
            return "fade green with shake";
        case BACK_ANIM_FADE_BLUE_WITH_SHAKE:
            return "fade blue with shake";
        default:
            return NULL;
    }
}
