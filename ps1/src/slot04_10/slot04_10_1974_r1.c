/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

void update_sequence(Object *object);
void set_direction_index_from_buttons_group_a(Object *object);
int func_801418bc(Object *object);
u8 get_pressed_bit_8(Object *object);
int is_pressed_bits_in_table(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013caf0(Object *object, u8 a, u8 b);
u8 func_8013ccb4(Object *object, u8 a, u8 b);
u8 func_8013cd5c(Object *object, u8 a, u8 b);
int func_8013d0c8(Object *object);
int func_8013d0fc(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
int func_80141788(Object *object);
u8 try_action_clearing_both_flags(Object *object);
int is_not_state_2_five(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);
/* functions of other units of this module */
void load_velocities_from_table_2_slot04_10(Object *obj);
int func_801b2324_slot04_10(Object *obj);
int func_801b2200_slot04_10(Object *obj);
u8 try_start_move_0_meter_cost_slot04_10(Object *obj);
u8 func_801b1d00_slot04_10(Object *obj);
u8 func_801b1d8c_slot04_10(Object *obj);
u8 try_start_move_7_with_dir_slot04_10(Object *obj);
u8 try_start_move_8_with_dir_slot04_10(Object *obj);
u8 try_start_move_6_with_dir_slot04_10(Object *obj);
u8 try_start_move_4_with_dir_slot04_10(Object *obj);
u8 try_start_move_5_slot04_10(Object *obj);
u8 func_801b20d4_slot04_10(Object *obj);

void update_sequence_hit_other_0b_by_mode_slot04_10(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;

    update_sequence(obj);
    if (o->mode == 1) {
        if (o->input_bits != 0) {
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, 0);
        } else {
            set_other_hit_reaction_state_0(obj, 0, 5, 0xb, 0, 0, 0);
        }
        obj->state_3 = 4;
    }
}

void update_sequence_or_finish_sequence_3_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        load_velocities_from_table_2_slot04_10(obj);
    }
}

void select_slot04_10_action_from_inputs_slot04_10(Object *obj) {
    if (get_pressed_bit_8(obj) && (u8)func_801b2324_slot04_10(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_move_0_meter_cost_slot04_10(obj)) return;
    if (func_8013cd5c(obj, 0, 0x22) && func_801b1d00_slot04_10(obj)) return;
    if ((u8)func_8013caf0(obj, 1, 0x15) && func_801b1d8c_slot04_10(obj)) return;
    if (func_8013ccb4(obj, 2, 0x21) && try_start_move_7_with_dir_slot04_10(obj)) return;
    if (func_8013ccb4(obj, 3, 0x23) && try_start_move_8_with_dir_slot04_10(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 4) && try_start_move_6_with_dir_slot04_10(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0xd) && func_801b20d4_slot04_10(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 0xe) && (u8)func_801b2200_slot04_10(obj)) return;
    if ((u8)func_8013d0c8(obj) && try_start_move_4_with_dir_slot04_10(obj)) return;
    if ((u8)func_8013d0fc(obj)) try_start_move_5_slot04_10(obj);
}

u8 try_start_move_0_meter_cost_slot04_10(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if (is_not_state_2_five(obj)) {
                    if (func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 0;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                if (func_801418bc(obj)) {
                    r = 1;
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 8;
                    obj->state_3 = 0;
                    obj->field_15a = 0;
                }
            }
        }
        return r;
    }
    return 0;
}

u8 func_801b1d00_slot04_10(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30 && func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 0xb;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142718(obj);
        return 1;
    }
    return r;
}

u8 func_801b1d8c_slot04_10(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30 && func_80141788(obj) && try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 0xa;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
        return 1;
    }
    return r;
}

u8 try_start_move_7_with_dir_slot04_10(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

u8 try_start_move_8_with_dir_slot04_10(Object *obj) {
    u8 r = 0;

    if (*(u16 *)&obj->state_0 == 1 && obj->state_2 == 5) {
        if (obj->active_flag == 0) {
            if (try_action_clearing_both_flags(obj)) {
                r = 1;
                obj->field_15a = 8;
                obj->state_0 = 1;
                obj->state_1 = 0;
                obj->state_2 = 7;
                obj->state_3 = 0;
                obj->active_flag_3 = 1;
                obj->facing_side = obj->saved_facing;
                set_direction_index_from_buttons_group_b(obj);
                return 1;
            }
        }
    } else if (try_action_clearing_both_flags(obj)) {
        r++;
        obj->field_15a = 8;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 try_start_move_6_with_dir_slot04_10(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

u8 try_start_move_4_with_dir_slot04_10(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 4;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

u8 try_start_move_5_slot04_10(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 5;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    return 1;
}
