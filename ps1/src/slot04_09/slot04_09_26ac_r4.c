/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);

void count_down_then_hit_check_or_end_slot04_09(Object *obj) {
    Object *p;
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        p = obj->other;
        obj->state_3 = 6;
        p->field_6b = 0;
        set_sequence_by_side_and_mode(obj, 0x2a);
    } else {
        p = obj->other;
        p->field_6b = 8;
        if (check_hit_other_without_state_precheck(obj, -0x40, 0x40, 0x30, 0x31) != 0) {
            obj->state_3++;
            obj->step_timer = 0;
            obj->step_flags = obj->step_flags & 0xff00;
            obj->active_state = 1;
            obj->vel_x = 0x20000;
            obj->vel_y = 0x60000;
            obj->accel_x = 0;
            obj->accel_y = -0x6000;
            if (obj->facing_side == 0) {
                obj->vel_x = 0xfffe0000;
            }
            set_sequence_by_side_and_mode(obj, 0x29);
        }
        apply_velocity_and_acceleration_slot04_09(obj);
    }
}
