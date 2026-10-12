/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern ObjectFn data_801c6650_slot04_14[];
extern u8 data_801c6658_slot04_14[];
extern ObjectFn data_801c665c_slot04_14[];
extern u8 data_801c6664_slot04_14[];
extern s32 data_801c6668_slot04_14[];
extern ObjectFn data_801c6674_slot04_14[];
extern ObjectFn data_801c667c_slot04_14[];
extern ObjectFn data_801c7868_slot04_14[];

void set_sequence_entry_by_sound_side_table_slot04_14(Object *obj, u8 index);
void increment_state_0_slot04_14(Object *obj);
void set_sequence_from_variant_index_by_parent_side_slot04_14(Object *obj);
void copy_parent_position_offset_by_facing_slot04_14(Object *obj);
void reset_and_push_block_object_type_8(Slab172 *o);
void clear_object_and_push_on_free_stack(Object *o);
void call_state_2_handler_267c_unless_paused(Object *o);
void call_state_2_handler_2684_unless_paused(Object *o);

void move_until_near_ground_then_switch_state_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 t = obj->ground_y;
    t -= 0x10;
    if (o->pos_y >= t) {
        o->state_0 = 2;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
        set_sequence_entry_by_sound_side_table_slot04_14(o, 9);
    } else {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        *(s32 *)&o->pos_y_frac -= o->vel_y;
    }
}

void call_state_2_handler_unless_paused_slot04_14(Object *o) {
    call_state_2_handler_267c_unless_paused(o);
}

void call_state_2_handler_unless_paused_2_slot04_14(Object *o) {
    call_state_2_handler_2684_unless_paused(o);
}

void call_handler_by_state_2_4_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c6650_slot04_14[o->state_2](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void flip_facing_and_set_sequence_from_table_slot04_14(Object *o) {
    u8 t;
    o->countdown = 2;
    o->state_2++;
    o->facing_side ^= 1;
    t = data_801c6658_slot04_14[o->vel_table_index];
    if (t != 0) {
        set_sequence_entry_by_sound_side_table_slot04_14(o, t);
    }
    advance_sequence_checked(o);
}

void count_down_then_reset_states_negate_vel_x_slot04_14(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown == 0) {
        o->state_0 = 1;
        o->active_count = 1;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
        o->vel_y = 0;
        o->vel_x = -o->vel_x;
    }
    advance_sequence_checked(o);
}

void call_handler_by_state_2_5_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c665c_slot04_14[o->state_2](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void flip_facing_and_set_sequence_from_table_2_slot04_14(Object *o) {
    u8 t;
    o->countdown = 2;
    o->state_2++;
    o->facing_side ^= 1;
    t = data_801c6664_slot04_14[o->vel_table_index];
    if (t != 0) {
        set_sequence_entry_by_sound_side_table_slot04_14(o, t);
    }
    advance_sequence_checked(o);
}

void count_down_then_reset_states_load_vel_y_slot04_14(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown < 0) {
        o->state_0 = 1;
        o->active_count = 1;
        o->vel_x = -o->vel_x;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
        o->vel_y = data_801c6668_slot04_14[o->vel_table_index];
    }
    advance_sequence_checked(o);
}

void call_state_1_table_handler_then_cull_check_slot04_14(Object *o) {
    data_801c6674_slot04_14[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}

void advance_state_1_set_sequence_9_slot04_14(Object *o) {
    o->state_1 = o->state_1 + 1;
    set_sequence_entry_by_sound_side_table_slot04_14(o, 9);
}

void set_state_0_to_3_on_negative_step_flags_slot04_14(Object *o) {
    if ((s16)o->step_flags & 0x8000) {
        o->state_0 = 3;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
    }
    advance_sequence_checked(o);
}

void release_object_and_unlink_from_parent_slot04_14(Object *o) {
    Object *p = o->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void set_sequence_entry_by_sound_side_table_slot04_14(Object *obj, u8 index) {
    SequenceStep *e = seqs_a4_left[index + 0x20];
    if (obj->sound_side != 0) {
        e = seqs_154_right[index + 0x20];
    }
    set_sequence_entry(obj, e);
}

void call_handler_by_state_0_2_slot04_14(Object *o) {
    data_801c667c_slot04_14[o->state_0](o);
}

void init_object_from_parent_fields_slot04_14(Object *o) {
    Object *p = o->parent;
    o->state_0++;
    o->field_1c = p->field_1c;
    o->table_index = p->kind;
    o->field_0c = p->field_0c;
    o->image_row = p->image_row;
    o->field_0e = p->field_0e;
    o->spawn_x = 0x60;
    o->variant_index = 0;
    o->spawn_y = 0x1e0;
    o->ot_depth = 0;
}

void update_child_follow_parent_frame_param_slot04_14(Object *o) {
    Object *p = o->parent;
    int s;
    o->field_01 = 0;
    if (o->table_index != p->kind) {
        increment_state_0_slot04_14(o);
    } else {
        s = p->frame->frame_param;
        if (s == 0) {
            o->variant_index = 0;
        } else {
            copy_parent_position_offset_by_facing_slot04_14(o);
            o->field_01 = 1;
            if (o->variant_index != s) {
                o->variant_index = s;
                set_sequence_from_variant_index_by_parent_side_slot04_14(o);
            } else {
                advance_sequence_checked(o);
            }
        }
    }
}

void increment_state_0_slot04_14(Object *o) {
    o->state_0++;
}

void unlink_from_parent_and_release_object_slot04_14(Object *o) {
    Object *p = o->parent;
    if (p->linked_object == (u32)o) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}

void set_sequence_from_variant_index_by_parent_side_slot04_14(Object *o) {
    if (o->parent->side == 0) {
        set_sequence_from_table_5(o, o->variant_index, data_1f8000b4);
    } else {
        set_sequence_from_table_5(o, o->variant_index, data_1f800164);
    }
}

void copy_parent_position_offset_by_facing_slot04_14(Object *o) {
    Object *p = o->parent;
    o->pos_x = p->pos_x;
    o->pos_y = p->pos_y;
    o->facing_side = 0;
    if (p->facing_side != 0) {
        o->pos_x = o->pos_x - 5;
    }
}

void call_handler_by_state_0_3_slot04_14(Object *o) {
    data_801c7868_slot04_14[o->state_0](o);
}
