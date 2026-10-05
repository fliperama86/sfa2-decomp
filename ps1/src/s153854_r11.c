/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80155eac(u8 idx, s8 side) {
    Object *o;
    if (!(side & 0x80)) {
        o = &player_right;
        if (side) {
            o--;
        }
        {
            int v = func_80155f98(o->field_170, table_80181530[idx]);
            o->field_170 = v;
            if (v > 0x9999998) {
                o->field_170 = 0x9999999;
            }
        }
    }
}
