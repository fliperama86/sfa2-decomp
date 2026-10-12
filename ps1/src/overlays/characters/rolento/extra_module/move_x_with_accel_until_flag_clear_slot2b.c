/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void halve_vel_x_add_meter_set_sequence_slot2b(Object *obj);

void move_x_with_accel_until_flag_clear_slot2b(Object *obj) {
    Slot2bObj *o = (Slot2bObj *)obj;
    o->pos_x_hi += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (o->active_flag == 0) {
        obj->state_3++;
        obj->pos_x_frac = 0;
    }
    update_sequence(obj);
}

void update_sequence_or_finish_sequence_slot2b(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}
