/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);
extern ObjectFn data_801be1f0_slot04_0c[];
extern ObjectFn data_801be1f8_slot04_0c[];
extern ObjectFn data_801be204_slot04_0c[];
extern ObjectFn data_801be20c_slot04_0c[];

void clear_object_and_push_on_free_stack(Object *o);
void call_handler_by_input_dir_0_4_slot04_0c(Object *obj);
void call_table_function_by_state_3_15_slot04_0c(Object *obj);
void call_table_function_by_state_3_16_slot04_0c(Object *obj);
void call_handler_by_state_3_set_flag_2_slot04_0c(Object *obj);
void set_sequence_and_tables_from_direction(Object *obj);

void increment_state_0_slot04_0c(Object *obj, Object *parent) {
    obj->state_0++;
}

void clear_linked_object_and_release_slot04_0c(Object *o, Object *p) {
    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}

void call_handler_by_input_dir_0_3_slot04_0c(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_4_slot04_0c(obj);
    } else {
        call_handler_by_state_3_set_flag_2_slot04_0c(obj);
    }
}

void call_handler_by_input_dir_0_4_slot04_0c(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_table_function_by_state_3_15_slot04_0c(obj);
    } else {
        call_table_function_by_state_3_16_slot04_0c(obj);
    }
}

void call_table_function_by_state_3_15_slot04_0c(Object *obj) {
    data_801be1f0_slot04_0c[obj->state_3](obj);
}

void check_range_or_start_directional_sequence_slot04_0c(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_or_finish_on_flag_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_16_slot04_0c(Object *obj) {
    data_801be1f8_slot04_0c[obj->state_3](obj);
}

void enter_state_3_restore_facing_slot04_0c(Object *obj) {
    int one = 1;
    obj->state_3++;
    obj->active_flag_3 = one;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 4) {
        obj->state_3 = 2;
        set_sequence_and_tables_from_direction(obj);
        obj->field_278 = one;
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_or_finish_on_flag_2_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void update_sequence_with_flagged_x_shift_slot04_0c(Object *obj) {
    s16 v = obj->step_flags;
    if (v & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (v & 0xff) {
            obj->step_flags = v & 0xff00;
            if (obj->facing_side == 0) {
                *(s32 *)&obj->pos_x_frac += -0x80000;
            } else {
                *(s32 *)&obj->pos_x_frac += 0x80000;
            }
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_set_flag_2_slot04_0c(Object *obj) {
    obj->field_157 = 1;
    data_801be204_slot04_0c[obj->state_3](obj);
}

void advance_state_3_restore_facing_set_sequence_slot04_0c(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_finish_on_flag_3_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void set_sequence_from_direction_and_variant_slot04_0c(Object *obj) {
    int t = 0xc;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + t));
}

void call_handler_by_state_0_2_slot04_0c(Object *obj) {
    data_801be20c_slot04_0c[obj->state_0](obj);
}
