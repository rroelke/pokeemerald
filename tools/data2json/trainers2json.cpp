#include "nlohmann/fifo_map.hpp"
#include "nlohmann/json.hpp"

#define _(string) string
#define LEVEL_UP_END 0xFFFF

#include "gba/types.h"

#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/global.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/trainers.h"

// major hack to increase the length of item names
#undef ITEM_NAME_LENGTH
#define ITEM_NAME_LENGTH 32
#include "item.h"

#include "structs.h"
#include "text.h"

#include "build/item_names.h"
#include "build/move_names.h"
#include "build/trainer_srcid.h"
#include "build/trainer_classes.h"
#include "build/trainer_pics.h"

#include <iostream>


extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const struct Trainer gTrainers[];
extern const char *const encounterMusicSourceNames[];


// https://github.com/nlohmann/json/issues/485
template <class K, class V, class dummy_compare, class A>
using fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

using json = nlohmann::basic_json<fifo_map>;


json makeTrainer(const int i) {
    const auto &tsrc = gTrainers[i];

    json trainer = json::object();
    trainer["name"] = trainerSourceNames[i];
    trainer["display_name"] = reinterpret_cast<const char *>(tsrc.trainerName);
    trainer["class"] = trainerClassNames[tsrc.trainerClass];
    trainer["encounter_music"] = encounterMusicSourceNames[tsrc.encounterMusic_gender & 0x7F];
    if (tsrc.encounterMusic_gender & F_TRAINER_FEMALE) {
        trainer["gender"] = "female";
    }
    if (strcmp(trainerClassNames[tsrc.trainerClass], trainerPicNames[tsrc.trainerPic])) {
        trainer["picture"] = trainerPicNames[tsrc.trainerPic];
    }
    // picture - cannot reverse from source (TODO: consider template for this)
    if (tsrc.doubleBattle) {
        trainer["double_battle"] = true;
    }
    trainer["items"] = json::array();
    for (int ii = 0; ii < 4; ii++) {
        if (tsrc.items[ii] != ITEM_NONE) {
            trainer["items"].push_back(itemNames[tsrc.items[ii]]);
        }
    }

    // ai flags
    json ai = json::object();
    if (tsrc.aiFlags & AI_SCRIPT_CHECK_BAD_MOVE) {
        ai["check_bad_move"] = true;
    }
    if (tsrc.aiFlags & AI_SCRIPT_TRY_TO_FAINT) {
        ai["try_to_faint"] = true;
    }
    if (tsrc.aiFlags & AI_SCRIPT_CHECK_VIABILITY) {
        ai["check_viability"] = true;
    }
    if (tsrc.aiFlags & AI_SCRIPT_SETUP_FIRST_TURN) {
        ai["setup_first_turn"] = true;
    }
    if (tsrc.aiFlags & AI_SCRIPT_RISKY) {
        ai["risky"] = true;
    }
    if (tsrc.aiFlags & AI_SCRIPT_PREFER_STRONGEST_MOVE) {
        ai["prefer_strongest_move"] = true;
    }
    if (tsrc.aiFlags & AI_SCRIPT_PREFER_BATON_PASS) {
        ai["prefer_baton_pass"] = true;
    }
    if (tsrc.aiFlags & AI_SCRIPT_HP_AWARE) {
        ai["hp_aware"] = true;
    }
    trainer["ai"] = ai;

    json party = json::array();
    const bool heldItems = tsrc.partyFlags & F_TRAINER_PARTY_HELD_ITEM;
    const bool customMoves = tsrc.partyFlags & F_TRAINER_PARTY_CUSTOM_MOVESET;
    for (int p = 0; p < tsrc.partySize; p++) {
        json mon = json::object();
        if (heldItems && customMoves) {
            const auto &msrc = tsrc.party.ItemCustomMoves[p];
            mon["iv"] = msrc.iv;
            mon["level"] = msrc.lvl;
            mon["species"] = reinterpret_cast<const char *>(gSpeciesNames[msrc.species]);
            mon["held_item"] = itemNames[msrc.heldItem];
            mon["moves"] = json::array();
            for (int im = 0; im < MAX_MON_MOVES; im++) {
                if (msrc.moves[im] != MOVE_NONE) {
                    mon["moves"].push_back(moveNames[msrc.moves[im]]);
                }
            }
        } else if (heldItems) {
            const auto &msrc = tsrc.party.ItemDefaultMoves[p];
            mon["iv"] = msrc.iv;
            mon["level"] = msrc.lvl;
            mon["species"] = reinterpret_cast<const char *>(gSpeciesNames[msrc.species]);
            mon["held_item"] = itemNames[msrc.heldItem];
        } else if (customMoves) {
            const auto &msrc = tsrc.party.NoItemCustomMoves[p];
            mon["iv"] = msrc.iv;
            mon["level"] = msrc.lvl;
            mon["species"] = reinterpret_cast<const char *>(gSpeciesNames[msrc.species]);
            mon["moves"] = json::array();
            for (int im = 0; im < MAX_MON_MOVES; im++) {
                if (msrc.moves[im] != MOVE_NONE) {
                    mon["moves"].push_back(moveNames[msrc.moves[im]]);
                }
            }
        } else {
            const auto &msrc = tsrc.party.NoItemDefaultMoves[p];
            mon["iv"] = msrc.iv;
            mon["level"] = msrc.lvl;
            mon["species"] = reinterpret_cast<const char *>(gSpeciesNames[msrc.species]);
        }
        party.push_back(mon);
    }
    trainer["party"] = party;

    return trainer;
}


int main(int argc, char **argv) {
    /* add trainer data */
    json trainers = json::array();
    for (int i = 1; i < TRAINERS_COUNT; i++) {
        trainers.push_back(makeTrainer(i));
    }

    json root = json::object();
    root["trainers"] = trainers;

    std::cout << root.dump(4, ' ', false) << std::endl;
}
