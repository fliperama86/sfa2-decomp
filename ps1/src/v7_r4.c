/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* 0x4e is the high half of field_4c (s32 in the table); accessed as s16. */
/* The shared tail via goto reproduces the block order of the original. */
int func_8012f2b8(Object *o) {
    int side;
    s16 v;
    int bit;
    if (o->kind == 4 || o->kind == 0x12 || o->kind == 7) {
        if (o->pos_y < (s16)(o->field_70 - 0x20) && o->field_48 != 0 && (o->field_130 & 0x4000) == 0) {
            side = o->field_164;
            if (side == 0) return 0;
            v = ((s16 *)&o->field_4c)[1];
            if (v == 0) return 0;
            if (v >= 0) {
                if (side == 2) return o->field_c2 >> 15;
                return 0;
            }
            if (side == 1) {
                goto last;
            }
        }
    }
    return 0;
last:
    bit = o->field_c2 & 0x2000;
    return bit != 0;
}
