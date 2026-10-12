/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_advance_sequence_checked_slot06_0f(Object *obj);
int func_801ea77c_slot06_0f(Object *obj);
int is_other_object_state_attacking_with_hit_slot06_0f(Object *obj);
void set_sequence_from_other_side_table_slot06_0f(Object *obj, int idx);
void set_countdown_random_multiple_of_4_slot06_0f(Object *obj);

extern ObjectFn data_801eeffc_slot06_0f[];

void call_handler_by_state_2_4_slot06_0f(Object *obj) {
    data_801eeffc_slot06_0f[obj->state_2](obj);
}

void update_hold_release_sequence_with_sound_slot06_0f(Object *obj) {
    int idx;

    if (game_state.config->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
        idx = 5;
        if (*(s32 *)&game_state.config->object_ptr == (s32)game_state.other_object) {
            play_side_sound_at(obj, obj->sound_side, 0x12);
            idx = 4;
        }
        set_sequence_from_other_side_table_slot06_0f(obj, idx);
    }
    call_advance_sequence_checked_slot06_0f(obj);
}

void call_advance_sequence_checked_slot06_0f(Object *obj) {
    advance_sequence_checked(obj);
}

void set_states_to_1_0_0_0_random_countdown_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 0;
    obj->state_3 = 0;
    set_countdown_random_multiple_of_4_slot06_0f(obj);
}

void update_variant_by_relative_x_slot06_0f(Object *obj) {
    int idx = (s16)obj->pos_x > (s16)game_state.other_object->pos_x;

    if (obj->variant_index != idx) {
        obj->variant_index = idx;
        set_sequence_from_other_side_table_slot06_0f(obj, idx);
    }
}

int func_801ea77c_slot06_0f(Object *obj) {
    s16 v = *(s16 *)&game_state.other_object->other->state_0;

    if (v != 0x101) {
        return v == 0x103;
    }
    return 1;
}

void set_states_one_one_and_random_countdown_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 1;
    obj->state_2 = 0;
    obj->state_3 = 0;
    set_countdown_random_multiple_of_4_slot06_0f(obj);
}

int is_other_object_state_attacking_with_hit_slot06_0f(Object *obj) {
    s16 v = *(s16 *)&game_state.other_object->state_0;

    if (v == 0x101) {
        if (obj->hit_type != 0xff) {
            return 1;
        }
    } else if (v == 0x103) {
        return 1;
    }
    return 0;
}

void set_states_one_two_and_random_countdown_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 2;
    obj->state_2 = 0;
    obj->state_3 = 0;
    set_countdown_random_multiple_of_4_slot06_0f(obj);
}

int is_both_state_checks_true_slot06_0f(Object *obj) {
    if (func_801ea77c_slot06_0f(obj)) {
        return is_other_object_state_attacking_with_hit_slot06_0f(obj) != 0;
    }
    return 0;
}

void set_states_one_one_and_random_countdown_2_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 1;
    obj->state_2 = 0;
    obj->state_3 = 0;
    set_countdown_random_multiple_of_4_slot06_0f(obj);
}

int is_config_enabled_flag_set_slot06_0f(Object *obj) {
    return game_state.config->enabled_flag != 0;
}

void set_states_1_3_0_0_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 3;
    obj->state_2 = 0;
    obj->state_3 = 0;
}

void release_object_record_3_slot06_0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
