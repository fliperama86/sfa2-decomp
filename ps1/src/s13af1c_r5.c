/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void set_pair_hit_states_and_play_sound(Object *a, Object *b) {
    b->state_0 = 1;
    b->state_2 = 7;
    b->field_15a = 11;
    b->state_1 = 0;
    b->state_3 = 0;
    a->hit_skip_flag = 1;
    a->active_flag_3 = 0;
    b->active_flag_3 = 0;
    a->field_225 = 0;
    b->field_225 = 0;
    a->field_6b = 16;
    b->field_6b = 12;
    play_sound_at(b, b->side, 0x313);
    spawn_effect_at_other_offset(a, 2, -0x40, 0x51);
}

int call_slot_stage_handler_a(Object *object, u8 a, u8 b) {
    u8 r = call_slot_stage_handler_in_second_table(object, a, b);
    return r;
}

int call_slot_stage_if_both_dir_pairs_ok(Object *object, u8 a, u8 b) {
    u8 r = (is_not_both_0c_bits_in_input(object) && is_not_both_30_bits_in_input(object)) ? call_slot_stage_handler_in_second_table(object, a, b) : reset_slot_stage_and_count(object, a);
    return r;
}

int func_8013cb70(Object *object, u8 a, u8 b) {
    u8 r = (is_not_both_c0_bits_in_input(object)) ? call_slot_stage_handler_in_second_table(object, a, b) : reset_slot_stage_and_count(object, a);
    return r;
}

int func_8013cbdc(Object *object, u8 a, u8 b) {
    u8 r = (is_not_both_30_bits_in_input(object)) ? call_slot_stage_handler_in_second_table(object, a, b) : reset_slot_stage_and_count(object, a);
    return r;
}

int func_8013cc48(Object *object, u8 a, u8 b) {
    u8 r = (is_not_both_0c_bits_in_input(object)) ? call_slot_stage_handler_in_second_table(object, a, b) : reset_slot_stage_and_count(object, a);
    return r;
}

u8 call_slot_stage_handler_b(Object *object, u8 a, u8 b) {
    return call_slot_stage_handler_in_first_table_2(object, a, b);
}

u8 call_slot_stage_d4cc_if_both_dir_pairs_ok(Object *object, u8 a, u8 b) {
    return (is_not_both_0c_bits_in_input(object) && is_not_both_30_bits_in_input(object)) ? call_slot_stage_handler_in_first_table_2(object, a, b) : reset_slot_stage_and_count(object, a);
}

u8 func_8013cd5c(Object *object, u8 a, u8 b) {
    return (is_not_both_c0_bits_in_input(object)) ? call_slot_stage_handler_in_first_table_2(object, a, b) : reset_slot_stage_and_count(object, a);
}

int call_slot_stage_handler_a_2(Object *object, u8 a, u8 b) {
    u8 r = call_slot_stage_handler_in_third_table(object, a, b);
    return r;
}

int func_8013cdf0(Object *object, u8 a, u8 b) {
    u8 r = (is_not_both_c0_bits_in_input(object)) ? call_slot_stage_handler_in_third_table(object, a, b) : reset_slot_stage_and_count(object, a);
    return r;
}

int func_8013ce5c(Object *object, u8 a, u8 b) {
    u8 r = (is_not_both_30_bits_in_input(object)) ? call_slot_stage_handler_in_third_table(object, a, b) : reset_slot_stage_and_count(object, a);
    return r;
}

int func_8013cec8(Object *object, u8 a, u8 b) {
    u8 r = (is_not_both_0c_bits_in_input(object)) ? call_slot_stage_handler_in_third_table(object, a, b) : reset_slot_stage_and_count(object, a);
    return r;
}

u8 call_slot_stage_d4cc_if_both_dir_pairs_ok_2(Object *object, u8 a, u8 b) {
    return (is_not_both_30_bits_in_input(object) && is_not_both_0c_bits_in_input(object)) ? call_slot_stage_handler_in_third_table(object, a, b) : reset_slot_stage_and_count(object, a);
}

u8 call_slot_stage_handler_b_2(Object *object, u8 a, u8 b) {
    return call_slot_stage_handler_in_fourth_table(object, a, b);
}

int call_slot_stage_handler_a_3(Object *object, u8 a, u8 b) {
    u8 r = call_slot_stage_handler_in_fourth_table_2(object, a, b);
    return r;
}

u8 is_not_both_c0_bits_in_input(Object *object) {
    if (object->field_d8 != 0) {
        return ((object->pressed_bits | object->released_bits) & 0xc0) != 0xc0;
    }
    return 1;
}

u8 is_not_both_30_bits_in_input(Object *object) {
    if (object->field_d8 != 0) {
        return ((object->pressed_bits | object->released_bits) & 0x30) != 0x30;
    }
    return 1;
}

u8 is_not_both_0c_bits_in_input(Object *object) {
    if (object->field_d8 != 0) {
        return ((object->pressed_bits | object->released_bits) & 0xc) != 0xc;
    }
    return 1;
}

u8 reset_slot_stage_and_count(Object *object, u8 a) {
    object->slots[a].stage = 0;
    object->field_25c = object->field_25c + 1;
    return 1;
}
