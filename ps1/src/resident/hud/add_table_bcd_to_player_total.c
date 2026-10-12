/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void add_table_bcd_to_player_total(int idx_arg, int side_arg) {
    u8 idx = idx_arg;
    s8 side = side_arg;
    Object *o;
    if (!(side & 0x80)) {
        o = &player_right;
        if (side) {
            o--;
        }
        {
            int v = add_bcd(o->field_170, table_80181530[idx]);
            o->field_170 = v;
            if (v > 0x9999998) {
                o->field_170 = 0x9999999;
            }
        }
    }
}
