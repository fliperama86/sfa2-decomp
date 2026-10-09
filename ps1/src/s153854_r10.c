/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80155c90(Object *o) {
    u8 *dst;
    u8 *src;
    int i;
    o->field_21e = 0;
    src = data_80181360;
    dst = &o->field_15e;
    for (i = 0; i < 0x20; i++) {
        *dst++ = *src++;
    }
}

void func_80155cc8(int amount, s8 side) {
    Object *o;
    if (!(side & 0x80)) {
        o = &player_left;
        if (side) {
            o++;
        }
        if (o->field_cd == 0) {
            int v = func_80155f98(o->field_b8, amount);
            o->field_b8 = v;
            if (v > 0x9999998) {
                o->field_b8 = 0x9999999;
            }
        }
    }
}

void func_80155d4c(int idx_arg, int side_arg) {
    u8 idx = idx_arg;
    s8 side = side_arg;
    Object *o;
    if (!(side & 0x80)) {
        o = &player_left;
        if (side) {
            o++;
        }
        if (o->field_cd == 0) {
            int v = func_80155f98(o->field_b8, table_80181530[idx]);
            o->field_b8 = v;
            if (v > 0x9999998) {
                o->field_b8 = 0x9999999;
            }
        }
    }
}
