/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void init_compare_limit_and_copy_bytes(Object *o) {
    u8 *dst;
    u8 *src;
    int i;
    o->compare_limit = 0;
    src = data_80181360;
    dst = &o->accum_count;
    for (i = 0; i < 0x20; i++) {
        *dst++ = *src++;
    }
}

void add_bcd_score_to_player(int amount, s8 side) {
    Object *o;
    if (!(side & 0x80)) {
        o = &player_left;
        if (side) {
            o++;
        }
        if (o->mode_flag_2 == 0) {
            int v = add_bcd(o->score, amount);
            o->score = v;
            if (v > 0x9999998) {
                o->score = 0x9999999;
            }
        }
    }
}

void add_table_bcd_score_to_player(int idx_arg, int side_arg) {
    u8 idx = idx_arg;
    s8 side = side_arg;
    Object *o;
    if (!(side & 0x80)) {
        o = &player_left;
        if (side) {
            o++;
        }
        if (o->mode_flag_2 == 0) {
            int v = add_bcd(o->score, table_80181530[idx]);
            o->score = v;
            if (v > 0x9999998) {
                o->score = 0x9999999;
            }
        }
    }
}
