/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);

void move_and_stop_x_near_other_slot04_09(Object *obj) {
    apply_velocity_and_acceleration_slot04_09(obj);
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        if (obj->active_flag == 0) {
            goto clear;
        }
        if (obj->other->hit_type == 0xff) {
            goto clear;
        }
        obj->field_a3 = 0xff;
        obj->active_flag = 0;
    }
    if ((u16)(obj->other->pos_x - obj->pos_x + 0x24) < 0x49) {
clear:
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}
