/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
extern ObjectFn data_801dd3ac_slot05_06[];

void advance_state_3_on_mode_and_update_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->mode != 0) {
        o->state_3++;
        obj->mode = 0;
    }
    update_sequence(o);
}

void update_slide_or_reset_on_status_flag_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->status_flags & 0x80) {
        o->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(o);
    } else {
        if (obj->mode != 0) {
            obj->mode = 0;
            o->active_flag_4 = 0;
        }
        if (o->vel_x >= 0) {
            move_x_by_vel_x_facing_then_accelerate_slot05_06(o);
        }
        start_action_if_buttons_pressed(o);
        update_sequence(o);
    }
}

void call_table_function_by_state_3_10_slot05_06(Object *obj) {
    data_801dd3ac_slot05_06[obj->state_3](obj);
}

void start_state_with_countdown_2_and_sound_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->state_3++;
    obj->countdown_2 = 0x32;
    spawn_three_child_objects_at_object_spawn(o);
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    play_side_sound_at(o, o->side, 7);
    set_sequence_by_side_and_mode(o, 0x22);
}
