/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern int data_801c2090_slot04_07[];
extern int data_801c209c_slot04_07[];

void move_x_by_variant_index_with_decel_stop_slot04_07(Object *obj);

void start_backward_move_flip_facing_slot04_07(Object *obj) {
    int a;
    int b;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = data_801c2090_slot04_07[obj->direction_index >> 1];
    obj->accel_x = -0x10000;
    obj->variant_index = obj->facing_side;
    obj->facing_side = obj->facing_side ^ 1;
    obj->vel_x = a;
    b = 0x60;
    if (obj->field_49 != 0) {
        b = 0x63;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + b);
}

void update_and_turn_around_when_mode_zero_slot04_07(Object *obj) {
    int a;

    update_sequence(obj);
    move_x_by_variant_index_with_decel_stop_slot04_07(obj);
    if (((Slot04aObj *)obj)->mode == 0) {
        obj->state_3++;
        obj->facing_side = obj->facing_side ^ 1;
        a = data_801c209c_slot04_07[obj->direction_index >> 1];
        obj->accel_x = -0x10000;
        obj->vel_x = a;
    }
}

void update_slide_sequence_or_end_slot04_07(Object *obj) {
    move_x_by_variant_index_with_decel_stop_slot04_07(obj);
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
        if (((Slot04aObj *)obj)->mode != 0) {
            obj->active_flag_4 = 0;
        }
    }
}
