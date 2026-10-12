/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c3284_slot04_15[];
extern u8 data_801c3c24_slot04_15[];
extern s32 data_801c3c2c_slot04_15[];
extern u8 data_801c3c44_slot04_15[];
extern ObjectFn data_801c3c4c_slot04_15[];
extern ObjectFn data_801c3c5c_slot04_15[];
extern ObjectFn data_801c3c70_slot04_15[];
extern ObjectFn data_801c3c78_slot04_15[];
extern ObjectFn data_801c3c80_slot04_15[];
extern ObjectFn data_801c3c90_slot04_15[];
extern ObjectFn data_801c3ca0_slot04_15[];
extern Object *data_801c3d7c_slot04_15;
extern BoxTables *data_801c34e4_slot04_15[];
extern BoxTables *data_801c3c08_slot04_15[];
extern BoxTables *data_801c3810_slot04_15[];
extern SequenceStep *data_801c2ca8_slot04_15[];
extern SequenceStep *data_801c0fc4_slot04_15[];

void reset_and_push_block_object_type_8(Slab172 *o);
void clear_object_and_push_on_free_stack(Object *o);
void clear_or_set_flag_by_pos_x_limit(Object *o);
void update_object_motion_unless_paused(Object *object);
void call_state_2_handler_267c_unless_paused(Object *object);
void call_state_2_handler_2684_unless_paused(Object *object);
void call_state_2_handler_268c_unless_paused(Object *object);
void call_handler_2694_by_state_2(Object *object);

void set_countdown_low_byte_from_table_slot04_15(Object *object);
void call_handler_by_state_1_slot04_15(Object *object);
void call_handler_by_state_1_then_offset_slot04_15(Object *object);
void select_box_tables_by_vel_index_and_health_slot04_15(Object *object);
void increment_state_0_slot04_15(Object *obj);
void set_variant_and_sequence_from_table_slot04_15(Object *o, u8 a);
void call_handler_by_state_1_unless_gate_slot04_15(Object *obj);
void follow_reference_object_by_frame_param_slot04_15(Object *obj);

void clear_object_tail_byte_block_slot04_15(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x4f; i >= 0; i--) {
        *p++ = z;
    }
}

void call_handler_by_state_0_slot04_15(Object *object) {
    data_801c3c4c_slot04_15[object->state_0](object);
}

void init_child_object_from_parent_slot04_15(Object *object) {
    Object *parent;
    int dx;
    int i;

    parent = object->parent;
    object->ot_depth = 0;
    object->state_0 = object->state_0 + 1;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    object->cycle_value = parent->cycle_value;
    ((Slot04bObj *)object)->data_table_ptr = data_801c3284_slot04_15;
    if (object->table_index != 0) {
        select_box_tables_by_vel_index_and_health_slot04_15(object);
    }
    object->active_state = 0;
    object->vel_y = 0;
    i = object->vel_table_index >> 1;
    i = data_801c3c2c_slot04_15[i];
    dx = 0x59;
    if (object->facing_side == 0) {
        i = -i;
        dx = -0x59;
    }
    object->vel_x = i;
    object->pos_x = object->pos_x + dx;
    object->pos_y = object->pos_y - 0x36;
    i = object->vel_table_index >> 1;
    object->variant_index_2 = data_801c3c24_slot04_15[i];
    if (parent->side == 0) {
        ((Slot04bObj *)object)->data_pointer = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)object)->data_pointer = *(int *)0x1f800158;
    }
    set_sequence_by_sound_side(object, i);
    set_countdown_low_byte_from_table_slot04_15(object);
    call_handler_by_state_1_slot04_15(object);
}

void set_countdown_low_byte_from_table_slot04_15(Object *object) {
    object->countdown = (s8)data_801c3c44_slot04_15[object->vel_table_index >> 1] | (object->countdown & 0xff00);
}

void call_handler_by_state_1_slot04_15(Object *object) {
    data_801c3c5c_slot04_15[object->state_1](object);
}

void update_or_switch_to_state_2_on_countdown_slot04_15(Object *object) {
    u16 t;

    if ((game_state.field_65 | game_state.field_a8) != 0) {
        goto call;
    }
    if (object->table_index != 0) {
        if ((s16)object->step_flags >= 0) {
            goto call;
        }
        goto set;
    }
    t = object->countdown - 1;
    object->countdown = t;
    if ((u8)t == 0) {
        goto set;
    }
call:
    update_object_motion_unless_paused(object);
    return;
set:
    object->state_0 = 2;
    object->state_1 = 0;
    object->state_2 = 0;
    object->state_3 = 0;
    call_handler_by_state_1_then_offset_slot04_15(object);
}

void call_handler_by_state_2_unless_paused_slot04_15(Object *object) {
    if ((game_state.field_65 | game_state.field_a8) != 0) {
        call_state_2_handler_267c_unless_paused(object);
    } else {
        data_801c3c70_slot04_15[object->state_2](object);
    }
}

void advance_state_2_set_box_tables_and_countdown_slot04_15(Object *object) {
    object->state_2 = object->state_2 + 1;
    select_box_tables_by_vel_index_and_health_slot04_15(object);
    ((Slot04bObj *)object)->countdown = 2;
    advance_sequence_checked(object);
}

void count_down_then_reset_states_slot04_15(Object *object) {
    ((Slot04bObj *)object)->countdown--;
    if (((Slot04bObj *)object)->countdown == 0) {
        object->active_count = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void call_state_2_handler_2684_wrapper_slot04_15(Object *object) {
    call_state_2_handler_2684_unless_paused(object);
}

void set_countdown_then_call_state_2_handler_a_slot04_15(Object *object) {
    set_countdown_low_byte_from_table_slot04_15(object);
    call_state_2_handler_268c_unless_paused(object);
}

void set_countdown_then_call_state_2_handler_b_slot04_15(Object *object) {
    set_countdown_low_byte_from_table_slot04_15(object);
    call_handler_2694_by_state_2(object);
}

void call_handler_by_state_1_then_offset_slot04_15(Object *obj) {
    data_801c3c78_slot04_15[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void clear_parent_link_and_set_sequence_offset_6_slot04_15(Object *obj) {
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

void set_state_0_to_3_on_sequence_end_slot04_15(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void release_object_and_unlink_from_parent_slot04_15(Object *o) {
    Object *p = o->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void select_box_tables_by_vel_index_and_health_slot04_15(Object *obj) {
    BoxTables **t;

    if (obj->vel_table_index == 6) {
        t = data_801c34e4_slot04_15;
    } else {
        t = data_801c3c08_slot04_15;
        if (obj->vel_table_index == 8) {
            t = data_801c3810_slot04_15;
        }
    }
    t = (BoxTables **)t[(s16)obj->health];
    obj->box_tables = (BoxTables *)t;
}

void call_handler_by_state_0_with_parent_slot04_15(Object *obj) {
    data_801c3d7c_slot04_15 = obj->parent;
    data_801c3c80_slot04_15[obj->state_0](obj);
}

void init_object_from_tracked_object_slot04_15(Object *obj) {
    Object *p;

    obj->state_0++;
    p = data_801c3d7c_slot04_15;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->table_index = p->kind;
    obj->field_0c = data_801c3d7c_slot04_15->field_0c;
    obj->image_row = data_801c3d7c_slot04_15->image_row;
    obj->variant_index = 0;
    follow_reference_object_by_frame_param_slot04_15(obj);
}

void follow_reference_object_by_frame_param_slot04_15(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801c3d7c_slot04_15;
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
                set_variant_and_sequence_from_table_slot04_15(obj, a);
            } else {
                advance_sequence_checked(obj);
            }
        }
    } else {
        increment_state_0_slot04_15(obj);
    }
}

void increment_state_0_slot04_15(Object *obj) {
    obj->state_0++;
}

void release_object_unlink_global_link_slot04_15(Object *o) {
    Object *p = data_801c3d7c_slot04_15;

    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}

void clear_variant_index_slot04_15(Object *obj) {
    obj->variant_index = 0;
}

void set_variant_and_sequence_from_table_slot04_15(Object *o, u8 a) {
    o->variant_index = a;
    set_sequence_entry(o, data_801c2ca8_slot04_15[a]);
}

void call_handler_by_state_0_2_slot04_15(Object *obj) {
    data_801c3c90_slot04_15[obj->state_0](obj);
}

void init_rise_toward_parent_then_call_handler_slot04_15(Object *obj) {
    obj->vel_y = 0xfffe0000;
    obj->accel_y = 0x1000;
    obj->state_0 = obj->state_0 + 1;
    obj->vel_x = (*(s32 *)&obj->parent->pos_x_frac - *(s32 *)&obj->pos_x_frac) >> 6;
    set_sequence_from_table_5(obj, 0, data_801c0fc4_slot04_15);
    call_handler_by_state_1_unless_gate_slot04_15(obj);
}

void call_handler_by_state_1_unless_gate_slot04_15(Object *obj) {
    if (game_state.gate_flag != 0) {
        obj->state_0 = 2;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        data_801c3ca0_slot04_15[obj->state_1](obj);
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}

void update_rise_until_parent_ground_offset_slot04_15(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    t = (u16)obj->parent->ground_y;
    t -= 0x55;
    if (obj->pos_y < t) {
        advance_sequence_checked(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->countdown = (obj->countdown & 0xff00) | 4;
    }
}

void count_down_then_set_upward_velocity_state_1_slot04_15(Object *obj) {
    u16 t = obj->countdown;
    t -= 1;
    obj->countdown = t;
    if ((u8)t == 0) {
        obj->vel_y = 0xffff0000;
        obj->accel_y = 0x2000;
        obj->state_1 = obj->state_1 + 1;
    }
}
