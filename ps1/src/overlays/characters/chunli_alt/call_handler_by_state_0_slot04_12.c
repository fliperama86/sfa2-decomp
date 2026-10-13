/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c4da0_slot04_12[];
extern ObjectFn data_801c4da8_slot04_12[];
extern s32 data_801c4db8_slot04_12[];
extern u8 data_801c4dc4_slot04_12[];
extern ObjectFn data_801c4dc8_slot04_12[];
extern SequenceStep *data_801c4de8_slot04_12[];
extern u8 data_801c4dec_slot04_12[];
extern u8 data_801c4e28_slot04_12[];
extern ObjectFn data_801c4e30_slot04_12[];
extern ObjectFn data_801c4e40_slot04_12[];
extern ObjectFn data_801c4e54_slot04_12[];
extern SequenceStep **data_801c4f08_slot04_12[];
extern SequenceStep **seqs_b4_left;
extern SequenceStep **seqs_164_right;
extern SequenceStep **data_1f8000f0;
extern SequenceStep **data_1f8001a0;

void reset_and_push_block_object_type_8(Slab172 *o);
void clear_or_set_flag_by_pos_x_limit(Object *o);

void call_handler_by_state_0_slot04_12(Object *obj);
void call_handler_by_state_1_slot04_12(Object *obj);
void advance_state_0_on_negative_step_flags_slot04_12(Object *obj);
void call_handler_by_state_1_then_offset_slot04_12(Object *obj);
void update_follower_by_parent_kind_slot04_12(Object *obj);
void follow_parent_frame_param_sequence_a_slot04_12(Object *obj, Object *p);
void follow_parent_frame_variant_sequence_slot04_12(Object *obj, Object *p);
void increment_state_0_slot04_12(Object *obj);

void call_handler_by_state_0_slot04_12(Object *obj) {
    data_801c4da8_slot04_12[obj->state_0](obj);
}

void init_child_projectile_from_parent_slot04_12(Object *obj) {
    Object *parent;
    u8 i;
    int k;

    parent = obj->parent;
    i = obj->vel_table_index;
    i >>= 1;
    obj->ot_depth = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c4da0_slot04_12;
    obj->field_0c = 0;
    obj->active_state = 0;
    obj->vel_y = 0;
    k = data_801c4db8_slot04_12[i];
    if (obj->facing_side != 0) {
        obj->vel_x = k;
        obj->pos_x = obj->pos_x + 0x58;
    } else {
        k = -k;
        obj->vel_x = k;
        obj->pos_x = obj->pos_x - 0x58;
    }
    obj->pos_y = obj->pos_y - 0x38;
    obj->variant_index_2 = data_801c4dc4_slot04_12[obj->vel_table_index >> 1];
    if (parent->side == 0) {
        ((Slot04bObj *)obj)->data_pointer = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->data_pointer = *(int *)0x1f800158;
    }
    set_sequence_by_sound_side(obj, obj->vel_table_index >> 1);
    call_handler_by_state_1_slot04_12(obj);
}

void call_handler_by_state_1_slot04_12(Object *obj) {
    data_801c4dc8_slot04_12[obj->state_1](obj);
}

void start_sequence_3_then_advance_slot04_12(Object *obj) {
    if (obj->state_1 != 0) {
        advance_state_0_on_negative_step_flags_slot04_12(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->parent->linked_object_3 = 0;
        set_sequence_by_sound_side(obj, 3);
    }
}

void advance_state_0_on_negative_step_flags_slot04_12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}

void release_object_and_unlink_from_parent_slot04_12(Object *obj) {
    Object *p = obj->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void call_handler_by_state_0_2_slot04_12(Object *obj) {
    data_801c4e30_slot04_12[obj->state_0](obj);
}

void init_child_object_from_parent_slot04_12(Object *obj) {
    Object *parent;

    obj->variant_index_2 = 0xff;
    obj->state_0 = obj->state_0 + 1;
    parent = obj->parent;
    obj->ot_depth = 0;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c4e28_slot04_12;
    ((Slot04bObj *)obj)->data_pointer = (int)data_801c4dec_slot04_12;
    obj->active_state = 0;
    obj->health = 0xff;
    set_sequence_entry(obj, data_801c4de8_slot04_12[0]);
    call_handler_by_state_1_then_offset_slot04_12(obj);
}

void call_handler_by_state_1_then_offset_slot04_12(Object *obj) {
    data_801c4e40_slot04_12[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_state_0_if_parent_frame_param_zero_slot04_12(Object *obj) {
    if (obj->parent->frame->frame_param == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}

void reset_states_and_health_slot04_12(Object *obj) {
    obj->state_0 = 1;
    obj->active_count = 1;
    obj->state_1 = 0;
    obj->state_2 = 0;
    obj->state_3 = 0;
    obj->health = 0xff;
}

void release_object_record_slot04_12(Object *obj) {
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot04_12(Object *obj) {
    data_801c4e54_slot04_12[obj->state_0](obj);
}

void init_follower_from_parent_kind_slot04_12(Object *obj) {
    Object *p = obj->parent;

    obj->state_0 = obj->state_0 + 1;
    obj->field_1c = p->field_1c;
    obj->state_3 = p->kind;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->field_0e = p->field_0e;
    obj->variant_index = 0;
    if (obj->table_index == 0) {
        if (p->side == 0) {
            data_801c4f08_slot04_12[0] = seqs_b4_left;
        } else {
            data_801c4f08_slot04_12[1] = seqs_164_right;
        }
    } else {
        if (p->side == 0) {
            data_801c4f08_slot04_12[2] = data_1f8000f0;
        } else {
            data_801c4f08_slot04_12[3] = data_1f8001a0;
        }
    }
    update_follower_by_parent_kind_slot04_12(obj);
}

void update_follower_by_parent_kind_slot04_12(Object *obj) {
    Object *p = obj->parent;

    obj->field_01 = 0;
    if (p->kind != obj->state_3) {
        increment_state_0_slot04_12(obj);
    } else if (obj->table_index == 0) {
        follow_parent_frame_param_sequence_a_slot04_12(obj, p);
    } else {
        follow_parent_frame_variant_sequence_slot04_12(obj, p);
    }
}
