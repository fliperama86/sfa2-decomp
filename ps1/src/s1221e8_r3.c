/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;
void func_80155eac(int idx, int side);
void func_80155d4c(int idx, int side);

void func_801229b4(Entity *entity) {
    u8 s0;
    u16 v;
    if (entity->field_64 != 0) {
        s0 = ref_first.p->field_255 + 2;
        v = func_80147158(ref_first.p);
        ref_first.p->field_bc = s0;
        func_80155eac(v, ref_first.p->field_65);
        func_80155d4c(v, ref_first.p->field_65);
    }
}

void func_80122a44(void) {
    table_8016e7d0[data_8018f5a0->field_50]();
    func_8012546c();
}

void func_80122a94(Entity *entity) {
    data_8018f5a0->field_50++;
    entity->field_116 = 0;
    entity->field_117 = 0x32;
    if (entity->field_64 != 0) {
        func_80123b20(entity->field_7c, entity);
    } else if (entity->field_4d == 0 && entity->field_a6 != 0) {
        entity->field_117 = 0x14;
    }
    if (entity->field_78 != 0) {
        entity->field_88 = entity->field_78->field_65;
    }
    func_80122b3c(entity);
}

void func_80122b3c(Entity *entity) {
    if (entity->field_64 != 0) {
        entity->field_11a = 0xff;
        entity->field_64--;
    } else {
        data_8018f5a0->field_50++;
        entity->field_6c = 5;
        if (entity->field_11a != 0) {
            func_8014f4d4(5, 1);
        }
    }
}

void func_80122bb0(Entity *entity) {
    u16 v;
    if (entity->field_4b != table_8016e7e0[entity->field_80]) {
        func_80122c5c(entity);
    } else {
        data_8018f5a0->field_50++;
        if (entity->field_30 != 0) {
            entity->field_ca = 0xa0;
        } else {
            entity->field_ca = 0x5a;
        }
        if (entity->field_4d == 0 && entity->field_a6 == 0) {
            func_80155f30(entity->field_7c);
        }
    }
}
