/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c2c00_slot04_0d[];
extern u8 data_801c35a0_slot04_0d[];
extern s32 data_801c35a8_slot04_0d[];
extern u8 data_801c35c0_slot04_0d[];
extern ObjectFn data_801c35cc_slot04_0d[];
extern ObjectFn data_801c35dc_slot04_0d[];
extern ObjectFn data_801c35f0_slot04_0d[];

Block172 *pop_block_from_pool(void);
void update_object_motion_unless_paused(Object *object);
void call_state_2_handler_267c_unless_paused(Object *object);
void call_state_2_handler_2684_unless_paused(Object *object);
void call_state_2_handler_268c_unless_paused(Object *object);
void call_handler_2694_by_state_2(Object *object);
void count_down_then_set_side_bit_slot04_0d(Object *object, GameState *g);
void set_countdown_low_from_table_slot04_0d(Object *object);
void call_handler_by_state_1_slot04_0d(Object *object);
void call_handler_by_state_1_then_offset_slot04_0d(Object *object);
void select_box_tables_by_vel_index_and_health_slot04_0d(Object *object);

void tick_countdown_and_update_sequence_slot04_0d(Object *object) {
    count_down_then_set_side_bit_slot04_0d(object, &game_state);
    update_sequence(object);
}

void count_down_then_set_side_bit_slot04_0d(Object *object, GameState *g) {
    if (((Slot04bObj *)object)->countdown != 0) {
        ((Slot04bObj *)object)->countdown--;
        if (((Slot04bObj *)object)->countdown == 0) {
            g->side_bits_2 |= 1 << object->side;
        }
    }
}

void update_sequence_spawn_offset_child_on_flag_slot04_0d(Object *object) {
    Object *p;
    int dx;
    u16 e;
    u8 d;

    update_sequence(object);
    if (object->step_flags & 0xff) {
        object->step_flags = object->step_flags & 0xff00;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x77;
            p->parent = object;
            p->field_0c = object->field_0c;
            p->image_row = object->image_row;
            p->field_0e = object->field_0e;
            p->facing_side = object->facing_side;
            p->field_1c = object->field_1c;
            e = object->cycle_value;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->cycle_value = e;
            d = object->image_row;
            p->field_02 = 5;
            p->mode_id = 0x20;
            p->image_row = d;
            p->field_90 = object->field_90;
            p->data_ptr_a = object->data_ptr_a;
            p->data_ptr_b = object->data_ptr_b;
            p->sound_side = object->sound_side;
            *(s32 *)&p->pos_x_frac = *(s32 *)&object->pos_x_frac;
            *(s32 *)&p->pos_y_frac = *(s32 *)&object->pos_y_frac;
            p->pos_y = p->pos_y - 0x63;
            dx = -0x3b;
            if (object->facing_side != 0) {
                dx = 0x3b;
            }
            p->pos_x = dx + p->pos_x;
        }
    }
    count_down_then_set_side_bit_slot04_0d(object, &game_state);
}

void clear_object_tail_byte_block_slot04_0d(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x4f; i >= 0; i--) {
        *p++ = z;
    }
}

void call_handler_by_state_0_slot04_0d(Object *object) {
    data_801c35cc_slot04_0d[object->state_0](object);
}

void init_child_object_from_parent_slot04_0d(Object *object) {
    Object *parent;
    int dx;
    int i;

    parent = object->parent;
    object->ot_depth = 0;
    object->state_0 = object->state_0 + 1;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    object->cycle_value = parent->cycle_value;
    ((Slot04bObj *)object)->data_table_ptr = data_801c2c00_slot04_0d;
    if (object->table_index != 0) {
        select_box_tables_by_vel_index_and_health_slot04_0d(object);
    }
    object->active_state = 0;
    object->vel_y = 0;
    i = object->vel_table_index >> 1;
    i = data_801c35a8_slot04_0d[i];
    dx = 0x59;
    if (object->facing_side == 0) {
        i = -i;
        dx = -0x59;
    }
    object->vel_x = i;
    object->pos_x = object->pos_x + dx;
    object->pos_y = object->pos_y - 0x36;
    i = object->vel_table_index >> 1;
    object->variant_index_2 = data_801c35a0_slot04_0d[i];
    if (parent->side == 0) {
        ((Slot04bObj *)object)->data_pointer = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)object)->data_pointer = *(int *)0x1f800158;
    }
    set_sequence_by_sound_side(object, i);
    set_countdown_low_from_table_slot04_0d(object);
    call_handler_by_state_1_slot04_0d(object);
}

void set_countdown_low_from_table_slot04_0d(Object *object) {
    object->countdown = *(u16 *)(data_801c35c0_slot04_0d + (object->vel_table_index & 0xfe)) | (object->countdown & 0xff00);
}

void call_handler_by_state_1_slot04_0d(Object *object) {
    data_801c35dc_slot04_0d[object->state_1](object);
}

void update_or_switch_to_state_2_on_countdown_slot04_0d(Object *object) {
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
    call_handler_by_state_1_then_offset_slot04_0d(object);
}

void call_handler_by_state_2_unless_paused_slot04_0d(Object *object) {
    if ((game_state.field_65 | game_state.field_a8) != 0) {
        call_state_2_handler_267c_unless_paused(object);
    } else {
        data_801c35f0_slot04_0d[object->state_2](object);
    }
}

void advance_state_2_set_box_tables_and_countdown_slot04_0d(Object *object) {
    object->state_2 = object->state_2 + 1;
    select_box_tables_by_vel_index_and_health_slot04_0d(object);
    ((Slot04bObj *)object)->countdown = 2;
    advance_sequence_checked(object);
}

void count_down_then_reset_states_slot04_0d(Object *object) {
    ((Slot04bObj *)object)->countdown--;
    if (((Slot04bObj *)object)->countdown == 0) {
        object->active_count = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void call_state_2_handler_2684_wrapper_slot04_0d(Object *object) {
    call_state_2_handler_2684_unless_paused(object);
}

void set_countdown_low_then_call_state_2_handler_slot04_0d(Object *object) {
    set_countdown_low_from_table_slot04_0d(object);
    call_state_2_handler_268c_unless_paused(object);
}

void set_countdown_low_then_call_alt_state_2_handler_slot04_0d(Object *object) {
    set_countdown_low_from_table_slot04_0d(object);
    call_handler_2694_by_state_2(object);
}
