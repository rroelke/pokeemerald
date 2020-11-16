#include "nlohmann/fifo_map.hpp"
#include "nlohmann/json.hpp"

#include "gba/types.h"

#include "constants/battle_ai.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_mons.h"
#include "constants/global.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#include "build/item_names.h"
#include "build/move_names.h"
#include "build/nature_names.h"

#include "structs.h"


extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u16 gBattleFrontierHeldItems[];
extern const struct FacilityMon gBattleFrontierMons[NUM_FRONTIER_MONS];


// https://github.com/nlohmann/json/issues/485
template <class K, class V, class dummy_compare, class A>
using fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

using json = nlohmann::basic_json<fifo_map>;


json makeFrontierMon(const int i) {
    json mon;
    mon["species"] = reinterpret_cast<const char *>(gSpeciesNames[gBattleFrontierMons[i].species]);
    mon["moves"] = json::array();
    for (int im = 0; im < MAX_MON_MOVES; im++) {
        if (gBattleFrontierMons[i].moves[im] != MOVE_NONE) {
            mon["moves"].push_back(moveNames[gBattleFrontierMons[i].moves[im]]);
        }
    }
    mon["item"] = itemNames[gBattleFrontierHeldItems[gBattleFrontierMons[i].itemTableId]];

    int nEvSpread = 0;
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_HP) {
        ++nEvSpread;
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_ATTACK) {
        ++nEvSpread;
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_DEFENSE) {
        ++nEvSpread;
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_SPEED) {
        ++nEvSpread;
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_SP_ATTACK) {
        ++nEvSpread;
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_SP_DEFENSE) {
        ++nEvSpread;
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_HP) {
        mon["evs"]["hp"] = (MAX_TOTAL_EVS / nEvSpread);
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_ATTACK) {
        mon["evs"]["attack"] = (MAX_TOTAL_EVS / nEvSpread);
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_DEFENSE) {
        mon["evs"]["defense"] = (MAX_TOTAL_EVS / nEvSpread);
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_SPEED) {
        mon["evs"]["speed"] = (MAX_TOTAL_EVS / nEvSpread);
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_SP_ATTACK) {
        mon["evs"]["special_attack"] = (MAX_TOTAL_EVS / nEvSpread);
    }
    if (gBattleFrontierMons[i].evSpread & F_EV_SPREAD_SP_DEFENSE) {
        mon["evs"]["special_defense"] = (MAX_TOTAL_EVS / nEvSpread);
    }

    mon["nature"] = natureNames[gBattleFrontierMons[i].nature];

    return mon;
}


int main(int argc, char **argv) {
    /* add trainer data */
    json mons = json::array();

    for (int i = 0; i < NUM_FRONTIER_MONS; i++) {
        mons.push_back(makeFrontierMon(i));
    }

    json root = json::object();
    root["pokemon"] = mons;

    std::cout << root.dump(4, ' ', false) << std::endl;
}
