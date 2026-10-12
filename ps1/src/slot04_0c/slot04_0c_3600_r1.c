/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bdb88_slot04_0c[];
extern u8 data_801be104_slot04_0c[];
extern ObjectFn data_801be19c_slot04_0c[];
extern u8 data_801be1ac_slot04_0c[];
extern u8 data_801be1b4_slot04_0c[];
extern ObjectFn data_801be1bc_slot04_0c[];
extern ObjectFn data_801be1d0_slot04_0c[];
extern ObjectFn data_801be1d8_slot04_0c[];

Block172 *pop_block_from_pool(void);
void func_8011ffdc(Object *object);
void set_sequence_by_side(Object *object, int arg);
void update_object_motion_unless_paused(Object *object);
void call_state_2_handler_2684_unless_paused(Object *object);
void call_state_2_handler_268c_unless_paused(Object *object);
void call_handler_2694_by_state_2(Object *object);
void set_countdown_2_from_table_slot04_0c(Object *object);
void call_handler_by_state_1_slot04_0c(Object *object);
void call_handler_by_state_1_slot01_slot04_0c(Object *object);
void set_box_tables_by_vel_index_and_health_slot04_0c(Object *object);

void update_countdown_set_config_side_bits_slot04_0c(Object *object) {
    s16 t = object->countdown;

    if (t != 0) {
        t = t - 1;
        object->countdown = t;
        if (t == 0) {
            game_state.config->side_bits |= object->side + 1;
        }
    }
    update_sequence(object);
}

void call_handler_by_state_2_3_slot04_0c(Object *object) {
    data_801bdb88_slot04_0c[object->state_2](object);
}

void advance_state_2_and_restore_facing_slot04_0c(Object *object) {
    object->state_2 = object->state_2 + 1;
    object->facing_side = object->saved_facing;
    set_sequence_by_side(object, 0);
}

void advance_state_2_unless_hold_flag_slot04_0c(Object *object) {
    if (game_state.config->hold_flag == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void start_countdown_and_set_sequence_slot04_0c(Object *object) {
    int arg = 0x28;

    object->countdown = 0x78;
    object->state_2 = object->state_2 + 1;
    if (game_state.config->table_index != 0) {
        arg = 0x29;
    }
    set_sequence_by_side(object, arg);
}

void spawn_child_on_step_flag_and_count_down_slot04_0c(Object *object) {
    Object *p;
    s16 t;

    if (object->step_flags & 0xff) {
        object->step_flags = object->step_flags & 0xff00;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xe;
            p->parent = object;
            p->spawn_x = object->spawn_x;
            p->spawn_y = object->spawn_y;
            p->image_row = object->image_row;
            p->mode_id = 0x20;
            p->sound_side = object->sound_side;
            p->field_90 = object->field_90;
            p->data_ptr_a = object->data_ptr_a;
            p->data_ptr_b = object->data_ptr_b;
        }
    }
    t = object->countdown;
    if (t != 0) {
        t = t - 1;
        object->countdown = t;
        if (t == 0) {
            game_state.config->side_bits |= object->side + 1;
        }
    }
    update_sequence(object);
}

void clear_object_byte_block_slot04_0c(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x57; i >= 0; i--) {
        *p++ = z;
    }
}

void call_handler_by_state_0_slot04_0c(Object *object) {
    data_801be19c_slot04_0c[object->state_0](object);
}

void init_child_object_from_parent_slot04_0c(Object *object) {
    Object *parent;
    int speed = 0x30000;

    object->variant_index_2 = 0xff;
    object->state_0 = object->state_0 + 1;
    parent = object->parent;
    object->ot_depth = 0;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    ((Slot04bObj *)object)->data_table_ptr = data_801be104_slot04_0c;
    object->active_state = 0;
    object->vel_y = 0;
    if (object->table_index == 2) {
        set_box_tables_by_vel_index_and_health_slot04_0c(object);
    }
    if (object->facing_side == 0) {
        object->vel_x = 0xfffd0000;
        object->pos_x = object->pos_x - 0x5a;
    } else {
        object->vel_x = speed;
        object->pos_x = object->pos_x + 0x5a;
    }
    object->pos_y = object->pos_y - 0x44;
    object->variant_index_2 = data_801be1ac_slot04_0c[object->vel_table_index >> 1];
    if (parent->side == 0) {
        ((Slot04bObj *)object)->data_pointer = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)object)->data_pointer = *(int *)0x1f800158;
    }
    func_80138070(object, object->vel_table_index >> 1);
    set_countdown_2_from_table_slot04_0c(object);
    call_handler_by_state_1_slot04_0c(object);
}

void set_countdown_2_from_table_slot04_0c(Object *object) {
    object->countdown_2 = data_801be1b4_slot04_0c[object->vel_table_index >> 1];
}

void call_handler_by_state_1_slot04_0c(Object *object) {
    data_801be1bc_slot04_0c[object->state_1](object);
}

void count_down_then_enter_state_2_slot04_0c(Object *object) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0 && --object->countdown_2 == 0) {
        object->state_0 = 2;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
        call_handler_by_state_1_slot01_slot04_0c(object);
    } else {
        update_object_motion_unless_paused(object);
    }
}

void call_state_2_handler_unless_paused_slot04_0c(Object *object) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        data_801be1d0_slot04_0c[object->state_2](object);
    }
    func_8011ffdc(object);
}

void func_801b3ca4_slot04_0c(Object *object) {
    object->state_2 = object->state_2 + 1;
    set_box_tables_by_vel_index_and_health_slot04_0c(object);
    object->countdown = 2;
    advance_sequence_checked(object);
}

void count_down_then_reset_states_and_advance_slot04_0c(Object *object) {
    object->countdown = (s16)object->countdown - 1;
    if ((s16)object->countdown == 0) {
        object->active_count = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
    advance_sequence_checked(object);
}

void func_801b3d3c_slot04_0c(Object *object) {
    call_state_2_handler_2684_unless_paused(object);
}

void func_801b3d5c_slot04_0c(Object *object) {
    set_countdown_2_from_table_slot04_0c(object);
    call_state_2_handler_268c_unless_paused(object);
}

void func_801b3d8c_slot04_0c(Object *object) {
    set_countdown_2_from_table_slot04_0c(object);
    call_handler_2694_by_state_2(object);
}

void call_handler_by_state_1_slot01_slot04_0c(Object *object) {
    data_801be1d8_slot04_0c[object->state_1](object);
    func_8011ffdc(object);
}
