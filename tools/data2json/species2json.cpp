#include "nlohmann/fifo_map.hpp"
#include "nlohmann/json.hpp"

#define _(string) string
#define LEVEL_UP_END 0xFFFF

#include "gba/types.h"

#include "constants/abilities.h"
#include "constants/global.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "data/pokemon/egg_moves.h"
#include "data/pokemon_graphics/icon_palette.h"

#include "pokedex.h"
#include "structs.h"
#include "text.h"

#include "build/ability_names.h"
#include "build/item_names.h"
#include "build/move_names.h"
#include "build/type_names.h"

#include <iostream>


extern const u16 gSpeciesToHoennPokedexNum[];
extern const u16 gSpeciesToNationalPokedexNum[];
extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u16 *const gLevelUpLearnsets[];
extern const u32 gTMHMLearnsets[][2];
extern const u32 gTutorLearnsets[];
extern const u16 gTutorMoves[];
extern const u16 sTMHMMoves[];
extern const struct PokedexEntry gPokedexEntries[];
extern const struct Evolution gEvolutionTable[NUM_SPECIES][EVOS_PER_MON];
extern const struct BaseStats gBaseStats[];
extern const struct MonCoords gMonBackPicCoords[];
extern const struct MonCoords gMonFrontPicCoords[];
extern const u8 gEnemyMonElevation[];
extern const u8 gMonFrontAnimIdsTable[];
extern const u8 gMonAnimationDelayTable[];
extern const u8 gSpeciesToBackAnimSet[];
extern const union AnimCmd *const *const gMonFrontAnimsPtrTable[];
extern const union AnimCmd *const *const gUnknownAnimsTable[];
extern const union AnimCmd sAnim_GeneralFrame0[];
extern const char *speciesSourceNames[];


// https://github.com/nlohmann/json/issues/485
template <class K, class V, class dummy_compare, class A>
using fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

using json = nlohmann::basic_json<fifo_map>;




json makeSpecimen(const int i) {
    json specimen = json::object();
    specimen["name"] = speciesSourceNames[i];
    if (strcmp(speciesSourceNames[i], reinterpret_cast<const char *>(gSpeciesNames[i]))) {
        specimen["display_name"] = reinterpret_cast<const char *>(gSpeciesNames[i]);
    }

    json base_stats = json::object();
    base_stats["hp"] = gBaseStats[i].baseHP;
    base_stats["attack"] = gBaseStats[i].baseAttack;
    base_stats["defense"] = gBaseStats[i].baseDefense;
    base_stats["speed"] = gBaseStats[i].baseSpeed;
    base_stats["special_attack"] = gBaseStats[i].baseSpAttack;
    base_stats["special_defense"] = gBaseStats[i].baseSpDefense;
    specimen["base_stats"] = base_stats;

    specimen["type"].push_back(typeNames[gBaseStats[i].type1]);
    if (gBaseStats[i].type1 != gBaseStats[i].type2) {
        specimen["type"].push_back(typeNames[gBaseStats[i].type2]);
    }

    specimen["abilities"] = json::array();
    if (gBaseStats[i].abilities[0] != ABILITY_NONE) {
        specimen["abilities"].push_back(abilityNames[gBaseStats[i].abilities[0]]);
    }
    if (gBaseStats[i].abilities[1] != ABILITY_NONE) {
        specimen["abilities"].push_back(abilityNames[gBaseStats[i].abilities[1]]);
    }

    specimen["catch_rate"] = gBaseStats[i].catchRate;
    specimen["experience_yield"] = gBaseStats[i].expYield;
    specimen["base_friendship"] = gBaseStats[i].friendship;
    if (gBaseStats[i].safariZoneFleeRate > 0) {
        specimen["safari_flee_rate"] = gBaseStats[i].safariZoneFleeRate;
    }
    specimen["growth_rate"] = growthRateToString(gBaseStats[i].growthRate);
    if (gBaseStats[i].genderRatio < 255) {
        specimen["pct_female"] = 100 * ((double)gBaseStats[i].genderRatio / 254);
    }

    json ev = json::object();
    if (gBaseStats[i].evYield_HP > 0) ev["hp"] = gBaseStats[i].evYield_HP;
    if (gBaseStats[i].evYield_Attack > 0) ev["attack"] = gBaseStats[i].evYield_Attack;
    if (gBaseStats[i].evYield_Defense > 0) ev["defense"] = gBaseStats[i].evYield_Defense;
    if (gBaseStats[i].evYield_Speed > 0) ev["speed"] = gBaseStats[i].evYield_Speed;
    if (gBaseStats[i].evYield_SpAttack > 0) ev["special_attack"] = gBaseStats[i].evYield_SpAttack;
    if (gBaseStats[i].evYield_SpDefense > 0) ev["special_defense"] = gBaseStats[i].evYield_SpDefense;
    specimen["ev_yield"] = ev;

    if (gBaseStats[i].item1 != ITEM_NONE || gBaseStats[i].item2 != ITEM_NONE) {
        specimen["item"].push_back(itemNames[gBaseStats[i].item1]);
        if (gBaseStats[i].item2 != ITEM_NONE) {
            specimen["item"].push_back(itemNames[gBaseStats[i].item2]);
        }
    }

    specimen["breeding"]["egg_cycles"] = gBaseStats[i].eggCycles;
    if (gBaseStats[i].eggGroup1 != EGG_GROUP_UNDISCOVERED) {
        specimen["breeding"]["egg_group"].push_back(eggGroupToString(gBaseStats[i].eggGroup1));
        if (gBaseStats[i].eggGroup2 != gBaseStats[i].eggGroup1) {
            specimen["breeding"]["egg_group"].push_back(eggGroupToString(gBaseStats[i].eggGroup2));
        }
    }

    if (i < NUM_SPECIES) {
        json learnsets = json::object();

        /* generate level up map */
        json levelup = json::array();
        for (int l = 0; gLevelUpLearnsets[i][l] != LEVEL_UP_END; l++) {
            json levelmove = json::object();
            levelmove[std::to_string(gLevelUpLearnsets[i][l] >> 9)] =
                moveNames[gLevelUpLearnsets[i][l] & 0x1FF];
            levelup.push_back(levelmove);
        }
        learnsets["levelup"] = levelup;

        /* generate egg moves */
        for (int e = 0; gEggMoves[e] != EGG_MOVES_TERMINATOR; e++) {
            if (gEggMoves[e] == (EGG_MOVES_SPECIES_OFFSET + i)) {
                json egg = json::array();
                while (gEggMoves[++e] < EGG_MOVES_SPECIES_OFFSET) {
                    egg.push_back(moveNames[gEggMoves[e]]);
                }
                learnsets["egg"] = egg;
            }
        }

        /* generate tm/hm array */
        json tmhm = json::array();
        for (int b = 0; b < 32; b++) {
            if (gTMHMLearnsets[i][0] & (1 << b)) {
                tmhm.push_back(moveNames[sTMHMMoves[b]]);
            }
        }
        for (int b = 0; b < 32; b++) {
            if (gTMHMLearnsets[i][1] & (1 << b)) {
                tmhm.push_back(moveNames[sTMHMMoves[32 + b]]);
            }
        }
        learnsets["tmhm"] = tmhm;

        /* generate move tutor array */
        json tutor = json::array();
        for (int b = 0; b < 32; b++) {
            if (gTutorLearnsets[i] & (1 << b)) {
                tutor.push_back(moveNames[gTutorMoves[b]]);
            }
        }
        learnsets["tutor"] = tutor;

        specimen["learnsets"] = learnsets;
    }

    json pokedex_number = json::object();
    pokedex_number["internal"] = i;
    pokedex_number["hoenn"] = gSpeciesToHoennPokedexNum[i - 1];
    pokedex_number["national"] = gSpeciesToNationalPokedexNum[i - 1];

    json pokedex = json::object();
    pokedex["number"] = pokedex_number;

    if (gSpeciesToNationalPokedexNum[i - 1] <= NATIONAL_DEX_COUNT) {
        const auto dexnum = gSpeciesToNationalPokedexNum[i - 1];
        json pokedex_entry = json::object();
        pokedex_entry["category"] = reinterpret_cast<const char *>(&gPokedexEntries[dexnum].categoryName[0]);
        pokedex_entry["height"] = gPokedexEntries[dexnum].height;
        pokedex_entry["weight"] = gPokedexEntries[dexnum].weight;
        pokedex_entry["scale"] = gPokedexEntries[dexnum].pokemonScale;
        pokedex_entry["offset"] = static_cast<s16>(gPokedexEntries[dexnum].pokemonOffset);
        pokedex_entry["trainer_scale"] = gPokedexEntries[dexnum].trainerScale;
        pokedex_entry["trainer_offset"] = static_cast<s16>(gPokedexEntries[dexnum].trainerOffset);
        pokedex_entry["text"] = reinterpret_cast<const char *>(gPokedexEntries[dexnum].description);

        pokedex["entry"]  = pokedex_entry;
    }
    specimen["pokedex"] = pokedex;

    /* add evolution */
    for (int e = 0; e < EVOS_PER_MON; e++) {
        if (gEvolutionTable[i][e].method == 0) {
            break;
        }
        json evo = json::object();
        switch (gEvolutionTable[i][e].method) {
            case EVO_FRIENDSHIP:
                evo["friendship"] = true;
                break;
            case EVO_FRIENDSHIP_DAY:
                evo["friendship_day"] = true;
                break;
            case EVO_FRIENDSHIP_NIGHT:
                evo["friendship_night"] = true;
                break;
            case EVO_LEVEL:
                evo["level"] = gEvolutionTable[i][e].param;
                break;
            case EVO_TRADE:
                evo["trade"] = true;
                break;
            case EVO_TRADE_ITEM:
                evo["trade_item"] = itemNames[gEvolutionTable[i][e].param];
                break;
            case EVO_ITEM:
                evo["item"] = itemNames[gEvolutionTable[i][e].param];
                break;
            case EVO_LEVEL_ATK_GT_DEF:
                evo["atk_gt_def"] = gEvolutionTable[i][e].param;
                break;
            case EVO_LEVEL_ATK_EQ_DEF:
                evo["atk_eq_def"] = gEvolutionTable[i][e].param;
                break;
            case EVO_LEVEL_ATK_LT_DEF:
                evo["atk_lt_def"] = gEvolutionTable[i][e].param;
                break;
            case EVO_LEVEL_SILCOON:
                evo["personality_lesslikely"] = gEvolutionTable[i][e].param;
                break;
            case EVO_LEVEL_CASCOON:
                evo["personality_morelikely"] = gEvolutionTable[i][e].param;
                break;
            case EVO_LEVEL_NINJASK:
                evo["emerge"] = gEvolutionTable[i][e].param;
                break;
            case EVO_LEVEL_SHEDINJA:
                evo["shed"] = gEvolutionTable[i][e].param;
                break;
            case EVO_BEAUTY:
                evo["beauty"] = gEvolutionTable[i][e].param;
                break;
            default:
                assert(0);
                break;
        }
        evo["target"] = reinterpret_cast<const char *>(gSpeciesNames[gEvolutionTable[i][e].targetSpecies]);
        specimen["evolution"].push_back(evo);
    }

    specimen["body_color"] = bodyColorToString(gBaseStats[i].bodyColor);

    json gfx = json::object();
    gfx["icon_palette"] = gMonIconPaletteIndices[i];
    if (gBaseStats[i].noFlip) {
        gfx["flip"] = false;
    }

    gfx["back_pic"]["size"] = gMonBackPicCoords[i].size;
    gfx["back_pic"]["y_offset"] = gMonBackPicCoords[i].y_offset;
    if (gSpeciesToBackAnimSet[i]) {
        gfx["back_pic"]["animation"] = backAnimToString(gSpeciesToBackAnimSet[i]);
    }
    gfx["front_pic"]["size"] = gMonFrontPicCoords[i].size;
    gfx["front_pic"]["y_offset"] = gMonFrontPicCoords[i].y_offset;
    if (gEnemyMonElevation[i] > 0) {
        gfx["front_pic"]["elevation"] = gEnemyMonElevation[i];
    }
    gfx["front_pic"]["animation"]["id"] = gMonFrontAnimIdsTable[i - 1];
    if (gMonAnimationDelayTable[i - 1] > 0) {
        gfx["front_pic"]["animation"]["delay"] = gMonAnimationDelayTable[i - 1];
    }

    /* front animation */
    if (gMonFrontAnimsPtrTable[i] != gMonFrontAnimsPtrTable[i - 1]) {
        const bool useGeneralFrame = (i != SPECIES_CASTFORM);
        json frames = json::array();
        for (int f = (useGeneralFrame ? 1 : 0); gMonFrontAnimsPtrTable[i][f]
                && gMonFrontAnimsPtrTable[i][f] != sAnim_GeneralFrame0; f++)
        {
            json framesets = json::array();
            for (int fs = 0; gMonFrontAnimsPtrTable[i][f][fs].type != -1; fs++) {
                const auto &frame = gMonFrontAnimsPtrTable[i][f][fs].frame;
                json jframe = json::array({ frame.imageValue, frame.duration });
                if (frame.hFlip) jframe.push_back(frame.hFlip);
                if (frame.vFlip) jframe.push_back(frame.vFlip);
                framesets.push_back(jframe);
            }
            frames.push_back(framesets);
        }
        if (!useGeneralFrame) {
            gfx["front_pic"]["animation"]["general"] = false;
        }
        gfx["front_pic"]["animation"]["frames"] = frames;
    } else {
        /* this is an OLD_UNOWN form */
        gfx["front_pic"]["animation"]["source"] = speciesSourceNames[SPECIES_OLD_UNOWN_B];
    }

    /* unknown animation */
    {
        json frames = json::array();
        for (int f = 1; gUnknownAnimsTable[i][f]
                && gUnknownAnimsTable[i][f] != sAnim_GeneralFrame0; f++)
        {
            json framesets = json::array();
            for (int fs = 0; gUnknownAnimsTable[i][f][fs].type != -1; fs++) {
                const auto &frameset = gUnknownAnimsTable[i][f][fs].frame;
                framesets.push_back(json::array({ frameset.imageValue, frameset.duration, frameset.hFlip, frameset.vFlip }));
            }
            frames.push_back(framesets);
        }
        if (!frames.empty()) {
            gfx["unknown"]["animation"] = frames;
        }
    }

    specimen["graphics"] = gfx;

    return specimen;
}




int main(int argc, char **argv) {
    /* add pokemon data */
    json species = json::array();
    for (int i = 1; i < NUM_SPECIES; i++) {
        species.push_back(makeSpecimen(i));
    }

    json root = json::object();
    root["species"] = species;

    std::cout << root.dump(4, ' ', false) << std::endl;
}
