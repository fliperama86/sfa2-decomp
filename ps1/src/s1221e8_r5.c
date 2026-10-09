/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80122f6c(GameState *state) {
    table_8016e7e8[((HudBig *)data_8018f5a0)->field_50](state);
    func_8012546c();
}

void func_80122fbc(GameState *state) {
    Entity *entity = (Entity *)state;
    if (entity->field_2f == 0) {
        ((HudBig *)data_8018f5a0)->field_50++;
        entity->field_ca = 0x1e;
        func_80156084();
        func_80156094();
        func_8012304c(entity);
    } else {
        ((HudBig *)data_8018f5a0)->field_50 = 5;
        entity->field_ca = 0xa0;
        func_8012332c(state);
    }
}

void func_8012304c(Entity *entity) {
    if (entity->field_104 < 0x500) {
        entity->field_108 = 4;
    } else {
        entity->field_108 = 0x100;
    }
}
