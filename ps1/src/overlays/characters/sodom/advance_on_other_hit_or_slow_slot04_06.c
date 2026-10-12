/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);

/* One local holds two values in turn: whether field_6b is set, and later the
   halved direction_index. With the test in place, with the shift in place, with
   both in place, and with a local for each, this function differs from the
   original in 3 instruction slots each time. */
void advance_on_other_hit_or_slow_slot04_06(Object *obj) {
    int v;
    if (obj->vel_x >= 0) {
        Object *other;
        move_x_by_vel_x_facing_then_accelerate_slot04_06(obj);
        other = obj->other;
        v = other->field_6b != 0;
        if (v && other->hit_type == 1) {
            if ((s16)other->health < 0) {
                goto set;
            }
            obj->state_3 += 3;
            obj->active_flag_3 = 1;
        }
        update_sequence(obj);
    } else {
    set:
        obj->state_3++;
        v = obj->direction_index >> 1;
        set_sequence_by_side_and_mode(obj, v + 0x45);
    }
}
