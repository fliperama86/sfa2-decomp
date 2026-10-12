/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void set_state_1_1_2_0_or_enter_state_1_6_2(Object *object) {
    if ((s16)object->health < 0) {
        enter_state_1_6_and_set_sequence(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->step_applied_flag = 0;
    }
}

void call_state_3_table_function_4(Object *object) {
    table_80171a3c[object->state_3](object);
}

void advance_state_3_and_start_hit_reaction(Object *object) {
    object->state_3 = object->state_3 + 1;
    object->active_flag_3 = 0;
    object->field_157 = 0;
    set_motion_from_vec_table(object);
    object->active_state = 0xff;
    store_image_row_for_hit_type(object);
    if (object->field_243 != 0) {
        start_hit_sequence_from_pairs_table(object);
        if (object->field_15b == 0) {
            object->vec_table_index = 6;
            set_motion_from_vec_table(object);
        } else {
            add_to_meter_clamped(object, 3);
            play_side_sound_1(object);
        }
    } else {
        play_side_sound_1(object);
    }
    if (object->field_163 != 0) {
        spawn_three_effect_objects_with_sound(object);
    }
    update_airborne_fall_and_land(object);
}
