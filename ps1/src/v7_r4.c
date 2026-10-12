/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* 0x4e is the high half of vel_x (s32 in the table); accessed as s16. */
/* The shared tail via goto reproduces the block order of the original. */
int is_pushing_toward_bound_in_air(Object *o) {
    int side;
    s16 v;
    int bit;
    if (o->kind == 4 || o->kind == 0x12 || o->kind == 7) {
        if (o->pos_y < (s16)(o->ground_y - 0x20) && o->variant_index != 0 && (o->input_bits_2 & 0x4000) == 0) {
            side = o->bound_push_flag;
            if (side == 0) return 0;
            v = ((s16 *)&o->vel_x)[1];
            if (v == 0) return 0;
            if (v >= 0) {
                if (side == 2) return o->input_bits >> 15;
                return 0;
            }
            if (side == 1) {
                goto last;
            }
        }
    }
    return 0;
last:
    bit = o->input_bits & 0x2000;
    return bit != 0;
}
