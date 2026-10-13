/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801ce9f0_slot04_17[];
extern ObjectFn data_801cea00_slot04_17[];
extern ObjectFn data_801cea08_slot04_17[];
extern s16 data_801cea10_slot04_17[];
extern ObjectFn data_801cea18_slot04_17[];
extern ObjectFn data_801cea24_slot04_17[];
extern ObjectRef ref_aux;
extern ObjectRef data_80190414;

void clear_object_and_push_on_free_stack(Object *o);
void set_sequence_and_tables_from_direction(Object *obj);
void reset_object_to_state_1_0_0_2_sequence_1(Object *obj);
void func_801b63fc(Object *obj);

void copy_kind_table_to_scratch_by_side_slot04_17(Object *obj);
void enable_active_flag_3_and_select_sequence_3_slot04_17(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_17(Object *obj);

void follow_ref_other_and_advance_sequence_slot04_17(Object *obj);
void set_position_from_ref_other_with_offset_slot04_17(Object *obj);
void set_kind_13_or_dispatch_by_input_dir_slot04_17(Object *obj);
void call_handler_by_state_3_clear_field_3_slot04_17(Object *obj);
void call_handler_by_state_3_set_flag_3_slot04_17(Object *obj);
void set_active_flag_3_and_call_direction_sequence_slot04_17(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_3_slot04_17(Object *obj);
void call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_17(Object *obj);
void set_kind_11_or_call_by_input_dir_slot04_17(Object *obj);

void call_handler_by_state_0_with_parent_refs_slot04_17(Object *obj) {
    ref_other.p = obj->parent;
    ref_aux.p = ref_other.p->parent;
    data_80190414.p = obj->other;
    data_801ce9f0_slot04_17[obj->state_0](obj);
}

void init_object_sequence_1_at_other_slot04_17(Object *obj) {
    obj->field_01 = 1;
    obj->image_row = 2;
    obj->field_0c = 0;
    obj->field_20 = 0;
    obj->table_index_2 = 0;
    obj->ot_depth = 0;
    obj->state_0++;
    obj->field_1c = ref_other.p->field_1c;
    set_position_from_ref_other_with_offset_slot04_17(obj);
    set_sequence_from_table_5(obj, 1, seqs_8017c7f8);
    follow_ref_other_and_advance_sequence_slot04_17(obj);
}

void follow_ref_other_and_advance_sequence_slot04_17(Object *obj) {
    set_position_from_ref_other_with_offset_slot04_17(obj);
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    advance_sequence_checked(obj);
}

void unlink_from_four_slots_and_release_slot04_17(Object *o) {
    Object **p = (Object **)ref_other.p->parent;
    int i;

    i = 4;
    do {
        if (p == (Object **)o) {
            *(u32 *)o = 0;
            break;
        }
        p++;
    } while (--i != 0);
    clear_object_and_push_on_free_stack(o);
}

void set_position_from_ref_other_with_offset_slot04_17(Object *obj) {
    obj->pos_x = (u16)ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y + 0x10;
    if (obj->table_index != 0) {
        obj->pos_x = obj->pos_x - 0xc;
        obj->pos_y = obj->pos_y - 6;
    }
}

void set_kind_13_or_dispatch_by_input_dir_slot04_17(Object *obj) {
    if (obj->attack_param_d != 0) {
        obj->kind = 0x13;
        copy_kind_table_to_scratch_by_side_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        set_kind_11_or_call_by_input_dir_slot04_17(obj);
    } else if (obj->input_dir_0 != 0) {
        call_handler_by_state_3_set_flag_3_slot04_17(obj);
    } else {
        call_handler_by_state_3_clear_field_3_slot04_17(obj);
    }
}

void call_handler_by_state_3_clear_field_3_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801cea00_slot04_17[obj->state_3](obj);
}

void restore_facing_enter_state_or_call_start_helper_slot04_17(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 0 || obj->hit_flag_a == 0 || (u8)try_force_other_state_if_box_in_range(obj, -0x17, 0x14) == 0) {
        set_active_flag_3_and_call_direction_sequence_slot04_17(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}

void update_sequence_reset_state_3_on_check_slot04_17(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        call_reset_object_to_state_1_0_sequence_0_3_slot04_17(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_set_flag_3_slot04_17(Object *obj) {
    obj->field_157 = 1;
    data_801cea08_slot04_17[obj->state_3](obj);
}

void advance_state_3_restore_facing_slot04_17(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    set_active_flag_3_and_call_direction_sequence_slot04_17(obj);
}

void update_sequence_reset_state_3_on_check_2_slot04_17(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_17(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void switch_to_kind_or_start_meter_sequence_slot04_17(Object *object) {
    int t;

    if (object->attack_param_d != 0) {
        object->kind = 0x13;
        copy_kind_table_to_scratch_by_side_slot04_17(object);
        select_box_tables(object);
        build_metrics(object);
        func_801b63fc(object);
        return;
    }
    if (object->limit_lo & 1) {
        object->input_dir_second = 2;
    }
    object->state_3 = 3;
    object->input_dir_0 = 4;
    object->active_flag_3 = 1;
    add_to_meter_clamped(object, *(s16 *) ((char *) data_801cea10_slot04_17 + (object->direction_index & 0xfe)));
    t = 0xc;
    if (object->variant_index != 0) {
        t = 0x12;
    }
    if (object->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(object, (object->direction_index >> 1) + t);
}

void set_active_flag_3_and_call_direction_sequence_slot04_17(Object *object) {
    object->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(object);
}

void call_reset_object_to_state_1_0_sequence_0_3_slot04_17(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_17(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void set_kind_11_or_call_by_input_dir_slot04_17(Object *obj) {
    if (obj->attack_param_d == 0) {
        obj->kind = 0x11;
        copy_kind_table_to_scratch_by_side_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        set_kind_13_or_dispatch_by_input_dir_slot04_17(obj);
    } else {
        data_801cea18_slot04_17[obj->input_dir_0 >> 1](obj);
    }
}

void call_handler_by_state_3_clear_field_4_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801cea24_slot04_17[obj->state_3](obj);
}

void update_state_or_switch_to_state_1_2_slot04_17(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x17, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        enable_active_flag_3_and_select_sequence_3_slot04_17(obj);
        shift_pos_x_by_step_flags_low_bits_slot04_17(obj);
    }
}
