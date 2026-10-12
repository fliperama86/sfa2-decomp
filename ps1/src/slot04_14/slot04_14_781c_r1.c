/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7930_slot04_14[];
extern u8 data_801c7938_slot04_14[];
extern ObjectFn data_801c7948_slot04_14[];
extern u8 data_801c7950_slot04_14[];
extern s32 data_801c7960_slot04_14[];
extern ObjectFn data_801c799c_slot04_14[];
extern ObjectFn data_801c79a4_slot04_14[];
extern ObjectFn data_801c7f34_slot04_14[];
extern void *data_801c7864_slot04_14[];
extern void *data_801c784c_slot04_14[];
extern void *data_801c7858_slot04_14[];
extern void *data_801c7838_slot04_14[];
extern void *data_801c7804_slot04_14[];
extern void *data_801c7818_slot04_14[];
extern void *data_801c77dc_slot04_14[];
extern void *data_801c7744_slot04_14[];
extern void *data_801c7784_slot04_14[];
extern void *data_801c7704_slot04_14[];
extern void *data_801c766c_slot04_14[];
extern void *data_801c76ac_slot04_14[];
extern SequenceStep *data_801c5af4_slot04_14[];

int dispatch_table_pick_by_table_index_slot04_14(Object *obj, Object *parent);
void select_data_table_group_a_slot04_14(Object *obj, u8 a);
void select_data_table_group_b_slot04_14(Object *obj, u8 a);
void select_data_table_group_c_slot04_14(Object *obj, u8 a);
void select_data_table_group_d_slot04_14(Object *obj, u8 a);
void set_data_table_ptr_from_table_slot04_14(Object *obj, void **tbl);
void set_object_states_0_to_3_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void set_sequence_a_by_sound_side_slot04_14(Object *obj, u8 a);
void follow_parent_step_flags_for_sequence_slot04_14(Object *obj);
void increment_state_0_2_slot04_14(Object *obj);
void set_sequence_from_variant_index_slot04_14(Object *obj);
void copy_parent_position_with_facing_offset_slot04_14(Object *obj);
void reset_and_push_block_object_type_8(Slab172 *o);
void call_state_2_handler_267c_unless_paused(Object *obj);
void call_state_2_handler_2684_unless_paused(Object *obj);
void clear_or_set_flag_by_pos_x_limit(Object *obj);

void move_until_near_ground_then_land_slot04_14(Object *obj) {
    if (obj->pos_y >= (s16)(obj->ground_y - 0x10)) {
        set_object_states_0_to_3_3_slot04_14(obj, 2, 0, 0, 0);
        obj->pos_y = obj->ground_y;
        set_sequence_a_by_sound_side_slot04_14(obj, 0x10);
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    }
}

void pick_table_if_indexed_then_call_state_2_handler_slot04_14(Object *obj) {
    if (obj->table_index != 0) {
        dispatch_table_pick_by_table_index_slot04_14(obj, obj->parent);
    }
    call_state_2_handler_267c_unless_paused(obj);
}

void load_velocities_from_table_2_slot04_14(Object *obj) {
    call_state_2_handler_2684_unless_paused(obj);
}

void call_handler_by_state_2_when_not_paused_slot04_14(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7930_slot04_14[obj->state_2](obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void flip_facing_and_set_sequence_from_table_3_slot04_14(Object *obj) {
    u8 a;

    obj->countdown = 2;
    obj->state_2++;
    obj->facing_side ^= 1;
    call_kind_9_handler_for_player(obj);
    a = data_801c7938_slot04_14[obj->vel_table_index];
    if (a != 0) {
        set_sequence_a_by_sound_side_slot04_14(obj, a);
    }
    advance_sequence_checked(obj);
}

void count_down_then_reverse_vel_x_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->active_count = 1;
        set_object_states_0_to_3_3_slot04_14(obj, 1, 0, 0, 0);
        obj->vel_x = -obj->vel_x;
        obj->vel_y = 0;
    }
    advance_sequence_checked(obj);
}

void call_handler_by_state_2_when_not_paused_2_slot04_14(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7948_slot04_14[obj->state_2](obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_state_2_flip_facing_table_image_row_slot04_14(Object *obj) {
    u8 a;

    obj->countdown = 2;
    obj->state_2++;
    obj->facing_side ^= 1;
    call_kind_9_handler_for_player(obj);
    a = data_801c7950_slot04_14[obj->vel_table_index];
    if (a == 0x1d || a == 0x1e || a == 0x1f) {
        obj->image_row++;
    }
    if (a != 0) {
        set_sequence_a_by_sound_side_slot04_14(obj, a);
    }
}

void count_down_then_reverse_vel_x_set_vel_y_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->active_count = 1;
        set_object_states_0_to_3_3_slot04_14(obj, 1, 0, 0, 0);
        obj->vel_x = -obj->vel_x;
        obj->vel_y = data_801c7960_slot04_14[obj->vel_table_index];
    }
    advance_sequence_checked(obj);
}

void call_handler_by_state_1_then_offset_slot04_14(Object *obj) {
    data_801c799c_slot04_14[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_state_1_set_sequence_by_table_index_slot04_14(Object *obj) {
    int a = 0x10;

    obj->state_1++;
    if (obj->table_index != 2 && obj->table_index != 8) {
        a = 0xf;
    }
    set_sequence_a_by_sound_side_slot04_14(obj, a);
}

void reset_states_on_negative_step_flags_then_advance_slot04_14(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        set_object_states_0_to_3_3_slot04_14(obj, 3, 0, 0, 0);
    }
    advance_sequence_checked(obj);
}

void unlink_from_parent_and_release_slot04_14(Object *obj) {
    Object *p = obj->parent;
    u8 n = p->link_flags - 1;

    p->linked_object_3 = 0;
    p->link_flags = n;
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

int dispatch_table_pick_by_table_index_slot04_14(Object *obj, Object *parent) {
    u8 a = obj->vel_table_index;

    if (obj->table_index == 6) {
        select_data_table_group_d_slot04_14(obj, a);
        return 1;
    }
    if (obj->table_index == 8) {
        select_data_table_group_c_slot04_14(obj, a);
        return 1;
    }
    if (obj->table_index == 4) {
        select_data_table_group_b_slot04_14(obj, a);
        return 1;
    }
    if (obj->table_index == 2) {
        select_data_table_group_a_slot04_14(obj, a);
        return 1;
    }
    return -1;
}

void select_data_table_group_a_slot04_14(Object *obj, u8 a) {
    void **t = data_801c7864_slot04_14;

    if (a == 3) {
        t = data_801c784c_slot04_14;
    }
    if (a == 4) {
        t = data_801c7858_slot04_14;
    }
    set_data_table_ptr_from_table_slot04_14(obj, t);
}

void select_data_table_group_b_slot04_14(Object *obj, u8 a) {
    void **t = data_801c7838_slot04_14;

    if (a == 6) {
        t = data_801c7804_slot04_14;
    }
    if (a == 7) {
        t = data_801c7818_slot04_14;
    }
    set_data_table_ptr_from_table_slot04_14(obj, t);
}

void select_data_table_group_c_slot04_14(Object *obj, u8 a) {
    void **t = data_801c77dc_slot04_14;

    if (a == 0xc) {
        t = data_801c7744_slot04_14;
    }
    if (a == 0xd) {
        t = data_801c7784_slot04_14;
    }
    set_data_table_ptr_from_table_slot04_14(obj, t);
}

void select_data_table_group_d_slot04_14(Object *obj, u8 a) {
    void **t = data_801c7704_slot04_14;

    if (a == 9) {
        t = data_801c766c_slot04_14;
    }
    if (a == 0xa) {
        t = data_801c76ac_slot04_14;
    }
    set_data_table_ptr_from_table_slot04_14(obj, t);
}

void set_data_table_ptr_from_table_slot04_14(Object *o, void **tbl) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->data_table_ptr = tbl[obj->field_5c];
}

void set_object_states_0_to_3_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}

void set_sequence_a_by_sound_side_slot04_14(Object *obj, u8 a) {
    if (obj->sound_side != 0) {
        set_sequence_entry(obj, seqs_154_right[a]);
    } else {
        set_sequence_entry(obj, seqs_a4_left[a]);
    }
}

void call_handler_by_state_0_4_slot04_14(Object *obj) {
    data_801c79a4_slot04_14[obj->state_0](obj);
}

void init_object_from_parent_fields_2_slot04_14(Object *obj) {
    Object *p = obj->parent;

    obj->state_0++;
    obj->field_1c = p->field_1c;
    obj->table_index = p->kind;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->field_0e = p->field_0e;
    obj->spawn_x = 0x60;
    obj->variant_index = 0;
    obj->spawn_y = 0x1e0;
    obj->field_90 = p->field_90;
    obj->data_ptr_a = p->data_ptr_a;
    obj->data_ptr_b = p->data_ptr_b;
    follow_parent_step_flags_for_sequence_slot04_14(obj);
}

void follow_parent_step_flags_for_sequence_slot04_14(Object *obj) {
    Object *p = obj->parent;
    u16 t;
    u16 u;
    u16 w;

    obj->field_01 = 0;
    if (obj->table_index != p->kind) {
        increment_state_0_2_slot04_14(obj);
    } else {
        w = p->step_flags;
        t = w & 0xff;
        u = t;
        if (u == 0) {
            obj->variant_index = 0;
        } else if ((w & 0x80) != 0) {
            increment_state_0_2_slot04_14(obj);
        } else {
            copy_parent_position_with_facing_offset_slot04_14(obj);
            obj->field_01 = 1;
            if (obj->variant_index != u) {
                obj->variant_index = t;
                set_sequence_from_variant_index_slot04_14(obj);
            } else {
                advance_sequence_checked(obj);
                clear_or_set_flag_by_pos_x_limit(obj);
            }
        }
    }
}

void increment_state_0_2_slot04_14(Object *obj) {
    obj->state_0++;
}

void release_object_record_slot04_14(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_sequence_from_variant_index_slot04_14(Object *obj) {
    set_sequence_from_table_5(obj, (u8)(obj->variant_index - 1), data_801c5af4_slot04_14);
}

void copy_parent_position_with_facing_offset_slot04_14(Object *obj) {
    Object *p = obj->parent;

    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->facing_side = 0;
    if (p->facing_side != 0) {
        obj->pos_x = obj->pos_x - 0x18;
    }
}

void call_handler_by_state_0_5_slot04_14(Object *obj) {
    data_801c7f34_slot04_14[obj->state_0](obj);
}
