/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void enter_state_1_6_and_set_sequence(Object *object) {
    object->state_0 = 1;
    object->state_1 = 6;
    object->state_2 = 0;
    object->state_3 = 0;
    object->done_flag = 0;
    object->once_flag_2 = 0;
    object->flag_25a_set = 0;
    set_side_flag(object);
    game_state.other_object = object->other;
    if (game_state.other_object->field_167 < 0xb) {
        add_round_score_to_other_side(object);
    }
    set_sequence_by_side_and_mode_2(object, 0x1b);
}

unsigned char is_other_object_state_0_and_1_six_one(Object *object) {
    game_state.other_object = object->other;
    return *(u16 *)&game_state.other_object->state_0 == 0x601;
}

void set_win_state_and_store_result_byte(Object *object) {
    object->state_0 = 1;
    object->state_1 = 4;
    object->state_2 = 0;
    object->state_3 = 0;
    if ((s16)object->health == 0x90) {
        u8 count;

        count = object->repeat_count + 1;
        object->field_167 |= 0x80;
        object->repeat_count = count;
    }
    ((u8 *)object + object->win_count)[0xcf] = object->field_167;
}

void clear_flag_if_second_flag_set(Object *object) {
    if (object->field_251 != 0 && object->field_d8 != 0) {
        object->field_251 = 0;
    }
}

int is_button_4000_held_or_other_flag(Object *object) {
    unsigned char result;

    result = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        ref_other.p = object->other;
        if (object->hit_count_2 != 0) {
            result = ref_other.p->field_157 != 0;
        }
    }
    if (object->input_bits_2 & 0x4000) {
        result = 1;
    }
    return result;
}

void set_state_1_0_1_0_sequence_7(Object *object) {
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 1;
    object->state_3 = 0;
    object->field_157 = 0;
    set_sequence_by_side(object, 7);
}

int is_button_4000_pressed(Object *object) {
    int bits;

    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        ref_other.p = object->other;
        if (object->hit_count_2 != 0 && ref_other.p->field_157 != 0 &&
            object->field_15b == 0) {
            return 1;
        }
    }
    if (object->action_timer != 0) {
        bits = object->pressed_bits & 0x4000;
    } else {
        bits = object->input_bits_2 & 0x4000;
    }
    return bits != 0;
}

void set_state_1_0_1_2_sequence_4(Object *object) {
    object->state_0 = 1;
    object->state_2 = 1;
    object->state_1 = 0;
    object->state_3 = 2;
    object->field_157 = 0;
    set_sequence_by_side(object, 4);
}

int is_single_horizontal_input_held(Object *object) {
    int result;
    u16 bits;

    result = 0;
    bits = object->input_bits_2 & 0xa000;
    if (object->field_7e == 0) {
        if (bits == 0x8000 || bits == 0x2000) {
            result = 1;
        }
    }
    return result;
}

int is_only_horizontal_input_held(Object *object) {
    int result;
    u16 flags;

    flags = object->input_bits_2;
    result = 0;
    if (object->field_7e == 0) {
        if (flags == 0x8000 || flags == 0x2000) {
            result = 1;
        }
    }
    return result;
}

void start_move_state_1_0_2_1_with_sequence_by_input(Object *object) {
    int arg;

    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 2;
    object->state_3 = 1;
    set_vel_x_from_motion_speed_table(object);
    if (object->input_bits_2 & 0x8000) {
        arg = 2;
    } else {
        arg = 3;
    }
    set_sequence_by_side(object, arg);
}
