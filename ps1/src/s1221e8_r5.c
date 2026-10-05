/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;

void func_80122f6c(void) {
    table_8016e7e8[data_8018f5a0->field_50]();
    func_8012546c();
}

void func_80122fbc(Entity *entity) {
    if (entity->field_2f == 0) {
        data_8018f5a0->field_50++;
        entity->field_ca = 0x1e;
        func_80156084();
        func_80156094();
        func_8012304c(entity);
    } else {
        data_8018f5a0->field_50 = 5;
        entity->field_ca = 0xa0;
        func_8012332c(entity);
    }
}

void func_8012304c(Entity *entity) {
    if (entity->field_104 < 0x500) {
        entity->field_108 = 4;
    } else {
        entity->field_108 = 0x100;
    }
}
