/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_two_axis_positions_by_bit_flags_slot04_07(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int t;
    if ((obj->bit_flags & 1) == 0) {
        t = obj->value_a;
        t <<= 8;
        *(s32 *)&obj->field_5c += t;
        t = obj->value_c;
        obj->value_a += obj->delta_a;
        obj->value_c += obj->delta_c;
        t <<= 8;
        obj->field_60 -= t;
    }
    if ((obj->bit_flags & 2) == 0) {
        t = obj->value_b;
        t <<= 8;
        *(s32 *)&obj->data_pointer += t;
        t = obj->value_d;
        obj->value_b += obj->delta_b;
        obj->value_d += obj->delta_d;
        t <<= 8;
        *(s32 *)&obj->saved_pos_y -= t;
    }
}
