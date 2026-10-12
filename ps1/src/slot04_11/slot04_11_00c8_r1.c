/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b744c_slot04_11[];
extern u8 data_801b9e04_slot04_11[];
extern u32 data_801c7244_slot04_11[];
extern ObjectFn data_801c72e4_slot04_11[];
extern s8 data_801c72f4_slot04_11[];
extern ObjectFn data_801c7304_slot04_11[];
extern ObjectFn data_801c7314_slot04_11[];
extern ObjectFn data_801c731c_slot04_11[];
extern s16 data_801c7324_slot04_11[];

Object *pop_object_from_second_pool(void);
void set_sequence_by_side(Object *object, int index);
u8 get_random_byte(void);
int func_8013cb70(Object *object, u8 a, u8 b);
int call_slot_stage_handler_a(Object *object, u8 a, u8 b);
int call_slot_stage_handler_a_2(Object *object, u8 a, u8 b);
int is_pressed_bits_in_table(Object *object);
u8 get_pressed_bit_8(Object *object);
void update_sequence_with_input_check(Object *object);

/* functions of other units of this module */
void copy_kind_table_to_scratch_by_side_slot04_11(Object *obj);
void enable_active_flag_3_and_select_sequence_slot04_11(Object *obj);
int check_buttons_by_kind_and_flag_slot04_11(Object *obj);
int start_move_12_if_meter_ok_slot04_11(Object *obj);
int try_start_move_one_kind_seventeen_slot04_11(Object *obj);
int try_start_move_two_kind_nineteen_slot04_11(Object *obj);
int try_start_move_three_kind_nineteen_slot04_11(Object *obj);
int start_move_4_if_kind_11_meter_ok_slot04_11(Object *obj);
int start_move_5_if_kind_11_meter_ok_slot04_11(Object *obj);
int start_move_6_if_kind_13_meter_ok_slot04_11(Object *obj);
int start_move_7_if_kind_13_airborne_meter_ok_slot04_11(Object *obj);
int start_move_8_or_count_failure_slot04_11(Object *obj);
int start_move_9_if_allowed_slot04_11(Object *obj);
int try_start_move_ten_spending_meter_slot04_11(Object *obj);
int try_start_move_eleven_spending_meter_slot04_11(Object *obj);

int try_start_move_zero_kind_seventeen_slot04_11(Object *obj);
void call_handler_by_state_3_set_flag_slot04_11(Object *obj);
void call_handler_by_state_3_clear_field_slot04_11(Object *obj);

void init_object_copy_table_spawn_child_slot04_11(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b744c_slot04_11;
    obj->data_ptr_b = data_801b9e04_slot04_11;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c7244_slot04_11[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x11;
        c->table_index = 0;
        c->parent = obj;
        c->sound_side = obj->sound_side;
        ((Slot04bObj *)obj)->field_2c = (u32)c;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->image_row = obj->image_row;
    }
    obj->parent = 0;
    if (obj->kind == 0x13) {
        copy_kind_table_to_scratch_by_side_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
    }
}

void call_handler_by_state_2_slot04_11(Object *obj) {
    data_801c72e4_slot04_11[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_slot04_11(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_if_config_not_held_slot04_11(Object *obj) {
    game_state.config = &game_state_second;
    if (game_state.config->field_64 == 0 && game_state.config->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void start_random_sequence_and_set_timers_slot04_11(Object *obj) {
    int v;

    ((Slot04bObj *)obj)->field_47 = 0x3c;
    obj->state_2 = obj->state_2 + 1;
    if (game_state.config->countdown_2 == 0) {
        game_state.config->countdown_2 = 0x1e;
    }
    v = (s8)get_input_mapped_value_or_default(obj, data_801c72f4_slot04_11[get_random_byte() & 0xf]);
    v += 0x23;
    set_sequence_by_side(obj, v);
}

void count_down_field_and_set_side_bit_slot04_11(Object *obj) {
    if (((Slot04bObj *)obj)->field_47 != 0) {
        ((Slot04bObj *)obj)->field_47--;
        if (((Slot04bObj *)obj)->field_47 == 0) {
            game_state.config->side_bits |= obj->side + 1;
        }
    }
    update_sequence(obj);
}

void call_handler_by_state_2_2_slot04_11(Object *obj) {
    data_801c7304_slot04_11[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_2_slot04_11(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_if_not_hold_flag_slot04_11(Object *obj) {
    if (game_state.config->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void start_sequence_by_config_table_index_slot04_11(Object *obj) {
    int arg = 0x28;

    ((Slot04bObj *)obj)->field_47 = 0x78;
    obj->state_2 = obj->state_2 + 1;
    if (game_state.config->table_index != 0) {
        arg = 0x29;
    }
    set_sequence_by_side(obj, arg);
}

void count_down_field_and_set_side_bit_2_slot04_11(Object *obj) {
    if (((Slot04bObj *)obj)->field_47 != 0) {
        ((Slot04bObj *)obj)->field_47--;
        if (((Slot04bObj *)obj)->field_47 == 0) {
            game_state.config->side_bits |= obj->side + 1;
        }
    }
    update_sequence(obj);
}

void call_handler_by_input_dir_0_nonzero_slot04_11(Object *obj) {
    if (obj->input_dir_0 != 0) {
        call_handler_by_state_3_set_flag_slot04_11(obj);
    } else {
        call_handler_by_state_3_clear_field_slot04_11(obj);
    }
}

void call_handler_by_state_3_clear_field_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7314_slot04_11[obj->state_3](obj);
}

void advance_state_3_or_switch_to_state_1_2_slot04_11(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index == 0) {
        enable_active_flag_3_and_select_sequence_slot04_11(obj);
    } else if (obj->field_25f != 0 || (obj->input_bits_2 & 0xa000) == 0 ||
               (u8)try_force_other_state_if_box_in_range(obj, -0x14, 0xe) == 0) {
        enable_active_flag_3_and_select_sequence_slot04_11(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}

void call_update_sequence_with_input_check_slot04_11(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_handler_by_state_3_set_flag_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c731c_slot04_11[obj->state_3](obj);
}

void advance_state_3_and_set_direction_sequence_slot04_11(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    enable_active_flag_3_and_select_sequence_slot04_11(obj);
}

void call_update_sequence_with_input_check_2_slot04_11(Object *obj) {
    update_sequence_with_input_check(obj);
}

void set_state_3_start_direction_sequence_slot04_11(Object *obj) {
    int v;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    v = obj->direction_index;
    add_to_meter_clamped(obj, *(s16 *)((u8 *)data_801c7324_slot04_11 + (v & 0xfe)));
    v = 0xc;
    if (obj->variant_index != 0) {
        v = 0x12;
    }
    if (obj->input_dir_second != 0) {
        v += 3;
    }
    set_sequence_by_side_and_mode(obj, v + (obj->direction_index >> 1));
}

void select_slot04_11_action_from_inputs_slot04_11(Object *obj) {
    if (is_pressed_bits_in_table(obj) && start_move_12_if_meter_ok_slot04_11(obj)) return;
    if (call_slot_stage_if_both_dir_pairs_ok(obj, 0, 0x17) && start_move_7_if_kind_13_airborne_meter_ok_slot04_11(obj)) return;
    if (func_8013cb70(obj, 1, 0x15) && start_move_6_if_kind_13_meter_ok_slot04_11(obj)) return;
    if (func_8013cb70(obj, 2, 0x14) && start_move_5_if_kind_11_meter_ok_slot04_11(obj)) return;
    if (call_slot_stage_if_both_dir_pairs_ok(obj, 3, 0x16) && start_move_4_if_kind_11_meter_ok_slot04_11(obj)) return;
    if (call_slot_stage_handler_a_2(obj, 4, 1) && try_start_move_three_kind_nineteen_slot04_11(obj)) return;
    if (call_slot_stage_handler_a_2(obj, 5, 2) && try_start_move_two_kind_nineteen_slot04_11(obj)) return;
    if (call_slot_stage_handler_a(obj, 6, 5) && try_start_move_one_kind_seventeen_slot04_11(obj)) return;
    if (call_slot_stage_handler_a_3(obj, 7, 0) && try_start_move_zero_kind_seventeen_slot04_11(obj)) return;
    if (check_buttons_by_kind_and_flag_slot04_11(obj) && start_move_8_or_count_failure_slot04_11(obj)) return;
    if (obj->kind == 0x11 && (obj->pressed_bits & 2) && start_move_8_or_count_failure_slot04_11(obj)) return;
    if (obj->kind == 0x13 && (obj->pressed_bits & 1) && start_move_8_or_count_failure_slot04_11(obj)) return;
    if (get_pressed_bit_8(obj) && start_move_9_if_allowed_slot04_11(obj)) return;
    if (call_slot_stage_handler_a(obj, 8, 0xd) && try_start_move_ten_spending_meter_slot04_11(obj)) return;
    if (call_slot_stage_handler_a(obj, 9, 0xe)) try_start_move_eleven_spending_meter_slot04_11(obj);
}
