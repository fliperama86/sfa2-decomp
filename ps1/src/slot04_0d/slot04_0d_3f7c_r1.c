/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c35f8_slot04_0d[];
extern ObjectFn data_801c3600_slot04_0d[];
extern ObjectFn data_801c3610_slot04_0d[];
extern ObjectFn data_801c361c_slot04_0d[];
extern ObjectFn data_801c3624_slot04_0d[];
extern ObjectFn data_801c3630_slot04_0d[];
extern ObjectFn data_801c363c_slot04_0d[];
extern Object *data_801c36fc_slot04_0d;
extern BoxTables *data_801c2e60_slot04_0d[];
extern BoxTables *data_801c3584_slot04_0d[];
extern BoxTables *data_801c318c_slot04_0d[];
extern SequenceStep *data_801c2624_slot04_0d[];

void reset_and_push_block_object_type_8(Slab172 *o);
void clear_object_and_push_on_free_stack(Object *o);
void clear_or_set_flag_by_pos_x_limit(Object *o);
void set_sequence_and_tables_from_direction(Object *obj);
void increment_state_0_slot04_0d(Object *obj);
void follow_reference_object_by_frame_param_slot04_0d(Object *obj);
void set_variant_and_sequence_from_table_slot04_0d(Object *obj, u8 a);
void call_handler_by_direction_index_3_slot04_0d(Object *obj);
void call_handler_by_direction_index_4_slot04_0d(Object *obj);
void call_handler_by_input_dir_second_3_slot04_0d(Object *obj);
void call_handler_by_half_direction_index_slot04_0d(Object *obj);

void call_handler_by_state_1_then_offset_slot04_0d(Object *obj) {
    data_801c35f8_slot04_0d[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void clear_parent_link_and_set_sequence_offset_6_slot04_0d(Object *obj) {
    u8 t;

    obj->state_1++;
    obj->parent->linked_object_3 = 0;
    if (obj->table_index != 0) {
        t = obj->step_flags + 6;
    } else {
        t = (obj->vel_table_index >> 1) + 6;
    }
    set_sequence_by_sound_side(obj, t);
}

void set_state_0_to_3_on_sequence_end_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void release_object_and_unlink_from_parent_slot04_0d(Object *o) {
    Object *p = o->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void select_box_tables_by_vel_index_and_health_slot04_0d(Object *obj) {
    BoxTables **t;

    if (obj->vel_table_index == 6) {
        t = data_801c2e60_slot04_0d;
    } else {
        t = data_801c3584_slot04_0d;
        if (obj->vel_table_index == 8) {
            t = data_801c318c_slot04_0d;
        }
    }
    t = (BoxTables **)t[(s16)obj->health];
    obj->box_tables = (BoxTables *)t;
}

void call_handler_by_state_0_with_parent_slot04_0d(Object *obj) {
    data_801c36fc_slot04_0d = obj->parent;
    data_801c3600_slot04_0d[obj->state_0](obj);
}

void init_object_from_tracked_object_slot04_0d(Object *obj) {
    Object *p;

    obj->state_0++;
    p = data_801c36fc_slot04_0d;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->table_index = p->kind;
    obj->field_0c = data_801c36fc_slot04_0d->field_0c;
    obj->image_row = data_801c36fc_slot04_0d->image_row;
    obj->variant_index = 0;
    follow_reference_object_by_frame_param_slot04_0d(obj);
}

void follow_reference_object_by_frame_param_slot04_0d(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801c36fc_slot04_0d;
    if (obj->table_index == p->kind) {
        a = p->frame->frame_param;
        if (a == 0) {
            obj->variant_index = 0;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&p->pos_x_frac;
            *(s32 *)&obj->pos_y_frac = *(s32 *)&p->pos_y_frac;
            obj->facing_side = p->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != a) {
                set_variant_and_sequence_from_table_slot04_0d(obj, a);
            } else {
                advance_sequence_checked(obj);
            }
        }
    } else {
        increment_state_0_slot04_0d(obj);
    }
}

void increment_state_0_slot04_0d(Object *obj) {
    obj->state_0++;
}

void release_object_unlink_global_link_slot04_0d(Object *o) {
    Object *p = data_801c36fc_slot04_0d;

    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}

void clear_variant_index_slot04_0d(Object *obj) {
    obj->variant_index = 0;
}

void set_variant_and_sequence_from_table_slot04_0d(Object *o, u8 a) {
    o->variant_index = a;
    set_sequence_entry(o, data_801c2624_slot04_0d[a]);
}

void call_handler_by_input_dir_0_nonzero_2_slot04_0d(Object *obj) {
    if (obj->input_dir_0 != 0) {
        call_handler_by_half_direction_index_slot04_0d(obj);
    } else {
        call_handler_by_input_dir_second_3_slot04_0d(obj);
    }
}

void call_handler_by_input_dir_second_3_slot04_0d(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_4_slot04_0d(obj);
    } else {
        call_handler_by_direction_index_3_slot04_0d(obj);
    }
}

void call_handler_by_direction_index_3_slot04_0d(Object *obj) {
    data_801c3610_slot04_0d[obj->direction_index >> 1](obj);
}

void call_handler_by_state_3_slot04_0d(Object *object) {
    data_801c361c_slot04_0d[object->state_3](object);
}

void advance_state_3_check_range_or_start_sequence_slot04_0d(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x11, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_reset_state_3_on_check_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_2_slot04_0d(Object *object) {
    data_801c3624_slot04_0d[object->state_3](object);
}

void check_range_hit_or_start_directional_sequence_slot04_0d(Object *obj) {
    u8 one;

    obj->state_3++;
    if (obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x11, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        one = 1;
        obj->active_flag_3 = one;
        set_sequence_and_tables_from_direction(obj);
        obj->field_278 = one;
    }
}

void update_sequence_with_facing_x_nudge_slot04_0d(Object *obj) {
    s16 d;

    if (obj->step_flags & 0xff) {
        obj->state_3++;
        d = -0x18;
        if (obj->facing_side != 0) {
            d = 0x18;
        }
        *(s32 *)&obj->pos_x_frac += d;
    }
    if (consume_active_frame_flag(obj) & 0xff) {
        obj->state_3 = 0;
    }
    update_sequence(obj);
}

void update_sequence_or_reset_on_negative_flags_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_direction_index_4_slot04_0d(Object *obj) {
    data_801c3630_slot04_0d[obj->direction_index >> 1](obj);
}

void call_handler_by_state_3_3_slot04_0d(Object *object) {
    data_801c363c_slot04_0d[object->state_3](object);
}
