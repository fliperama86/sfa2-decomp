/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);
void apply_vertical_velocity_slot04_06(Object *obj);

void start_jump_with_fixed_velocity_slot04_06(Object *obj) {
    int v;

    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 8);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = 0x40000;
    obj->vel_y = 0x28000;
    obj->accel_x = 0;
    obj->accel_y = 0xffff6000;
    obj->active_state = 1;
    obj->active_flag_3 = 1;
    v = 0x48;
    if (obj->field_49 == 0) {
        v = 0x2a;
    }
    set_sequence_by_side_and_mode(obj, v);
}

void set_countdown_2_on_step_flag_else_update_slot04_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        ((Slot04aObj *)obj)->countdown_2 = 0x10;
    } else {
        update_sequence(obj);
    }
}

void update_air_motion_land_at_ground_with_countdown_slot04_06(Object *obj) {
    move_x_by_vel_x_facing_then_accelerate_slot04_06(obj);
    apply_vertical_velocity_slot04_06(obj);
    if (((Slot04aObj *)obj)->countdown_2 != 0) {
        if (--((Slot04aObj *)obj)->countdown_2 == 0) goto go;
    }
    if (obj->pos_y > obj->ground_y) {
go:
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        update_sequence(obj);
    }
}
