/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80122f6c(GameState *state) {
    table_8016e7e8[((HudBig *)data_8018f5a0)->state_index](state);
    func_8012546c();
}

void func_80122fbc(GameState *state) {
    Entity *entity = (Entity *)state;
    if (entity->mode_flag_a == 0) {
        ((HudBig *)data_8018f5a0)->state_index++;
        entity->countdown_timer = 0x1e;
        func_80156084();
        func_80156094();
        func_8012304c(entity);
    } else {
        ((HudBig *)data_8018f5a0)->state_index = 5;
        entity->countdown_timer = 0xa0;
        func_8012332c(state);
    }
}

void func_8012304c(Entity *entity) {
    if (entity->threshold_input < 0x500) {
        entity->selected_value = 4;
    } else {
        entity->selected_value = 0x100;
    }
}
