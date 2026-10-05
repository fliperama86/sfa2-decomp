/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int func_80142424(Object *object, Box32 *box, s16 elapsed) {
    s16 mask;
    s16 i, shift;
    mask = func_80142540(object, elapsed);
    mask = func_801425d8(object, mask);
    for (i = 11; i != -1; --i) {
        if (game_state.field_358->field_128) shift = shift_b[i];
        else shift = shift_a[i];
        mask = mask >> shift;
        if (mask) {
            object->field_128 = quad_table[shift * 4 + 0];
            object->field_129 = quad_table[shift * 4 + 1];
            object->field_12a = quad_table[shift * 4 + 2];
            object->field_219 = quad_table[shift * 4 + 3];
            return 1;
        }
    }
}

s16 func_80142540(Object *object, s16 value) {
    s16 mask;
    s16 i, limit;
    i = 0;
    if (object->side) game_state.field_30c = (Pair *)metrics_right;
    else game_state.field_30c = (Pair *)metrics_left;
    do {
        limit = game_state.field_30c->first;
        game_state.field_30c++;
        if (!(value < limit)) mask |= 1 << i;
        i++;
    } while (i < 12);
    return mask;
}
