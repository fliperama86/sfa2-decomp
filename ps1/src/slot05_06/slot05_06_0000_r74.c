/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_by_velocity_return_y_minus_ground_slot05_06(Object *obj);
void release_object_record_3_slot05_06(Object *obj);

void land_on_ground_and_advance_state_1_slot05_06(Object *obj) {
    if ((move_by_velocity_return_y_minus_ground_slot05_06(obj) << 16) > 0) {
        ((Slot04aObj *)obj)->timer = 8;
        obj->step_timer = 1;
        obj->state_1++;
        obj->pos_y = obj->ground_y;
        advance_sequence_checked(obj);
    }
}

void count_down_timer_advance_state_0_slot05_06(Object *obj) {
    ((Slot04aObj *)obj)->timer--;
    if (((Slot04aObj *)obj)->timer & 0x80) {
        obj->state_0++;
    }
}

void advance_sequence_checked_then_pos_x_limit_flag_slot05_06(Object *obj) {
    advance_sequence_checked(obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

int move_by_velocity_return_y_minus_ground_slot05_06(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return (s16)(obj->pos_y - obj->ground_y);
}

void load_velocities_from_table_7_slot05_06(Object *obj) {
    release_object_record_3_slot05_06(obj);
}

void release_object_record_3_slot05_06(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
