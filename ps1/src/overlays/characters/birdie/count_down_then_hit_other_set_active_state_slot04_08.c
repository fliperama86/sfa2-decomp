/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void count_down_then_hit_other_set_active_state_slot04_08(Object *obj) {
    Object *other;

    obj->countdown = (s16)obj->countdown - 1;
    if (obj->countdown & 0x8000) {
        other = obj->other;
        obj->state_3++;
        obj->facing_side ^= 1;
        other->field_15b = 1;
        other->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 2, 0xf, -0x200, 0, 0, 0);
        if (((Slot04aObj *)other)->field_5c < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        if (obj->facing_side == 0) {
            obj->vel_x = 0x18000;
        } else {
            obj->vel_x = -0x18000;
        }
        obj->vel_y = -0x50000;
        obj->accel_y = 0x5000;
        obj->active_state = 1;
        set_sequence_by_side(obj, 0x30);
    }
}
