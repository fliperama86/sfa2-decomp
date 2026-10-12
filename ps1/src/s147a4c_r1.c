/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_fns_cbd0_by_state_0(Block172 *block) {
    Object *object = (Object *)block;
    fns_cbd0[object->state_0](object);
}

void func_80147a8c(Object *object) {
    object->ot_depth = 0;
    object->state_0++;
    object->variant_index_2 = bytes_cbe0[object->table_index + object->variant_index];
    set_sequence_from_table_5(object, 0x13, (SequenceStep **)seqs_c7f8);
}

void call_handler_by_table_index_2(Object *object) {
    fns_cbe8[object->table_index](object);
}

void call_handler_by_state_1_then_follow_parent(Object *object) {
    fns_cbf4[object->state_1](object);
    set_position_from_parent(object);
    set_flag_and_push_if_mode_8(object);
}

void start_countdown_29_and_advance_sequence(Object *object) {
    object->countdown = 0x1d;
    object->state_1++;
    advance_sequence_checked(object);
}

void count_down_then_next_state_1_sequence(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->state_1++;
        set_sequence_from_table_5(object, 0x14, (SequenceStep **)seqs_c7f8);
    } else {
        advance_sequence_checked(object);
    }
}

void advance_sequence_until_flags_negative(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_1 = 0;
        object->state_0++;
    } else {
        advance_sequence_checked(object);
    }
}

void call_fns_cc00_by_state_1_then_push(Object *object) {
    fns_cc00[object->state_1](object);
    set_flag_and_push_if_mode_8(object);
}

void set_countdown_5_and_advance_state_1(Object *object) {
    object->countdown = 5;
    object->state_1++;
    advance_sequence_checked(object);
    set_velocity_accel_by_variant(object);
}

void set_velocity_accel_by_variant(Object *object) {
    if (object->variant_index != 0) {
        object->vel_x = 0xffc60000;
        object->vel_y = 0x180000;
        object->accel_x = 0x74000;
        object->accel_y = 0xfffd0000;
    } else {
        object->vel_x = 0x3a0000;
        object->vel_y = 0xffe80000;
        object->accel_x = 0xfff8c000;
        object->accel_y = 0x30000;
    }
    set_position_from_parent_with_velocity(object);
}

void count_down_reload_7_advance_sequence(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->countdown = 7;
        object->state_1++;
    }
    advance_sequence_checked(object);
}

void count_down_then_advance_state_1(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->countdown = 7;
        object->state_1++;
        set_velocity_accel_by_variant(object);
    } else {
        set_position_from_parent_with_velocity(object);
    }
}

void call_count_down_then_advance_state_1(Object *object) {
    count_down_then_advance_state_1(object);
}

void count_down_follow_parent_then_advance_state(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        if (object->active_state != 0) {
            advance_state_0_reset_state_1(object);
        } else {
            object->state_1++;
            set_position_from_parent(object);
            set_sequence_from_table_5(object, 0x14, (SequenceStep **)seqs_c7f8);
        }
    } else {
        set_position_from_parent_with_velocity(object);
    }
}

void advance_sequence_until_flags_negative_2(Object *object) {
    if ((s16)object->step_flags < 0) {
        advance_state_0_reset_state_1(object);
    } else {
        advance_sequence_checked(object);
    }
}

void advance_state_0_reset_state_1(Object *object) {
    object->state_0++;
    object->state_1 = 0;
}

void call_fns_cc18_by_state_1_then_push(Object *object) {
    fns_cc18[object->state_1](object);
    set_flag_and_push_if_mode_8(object);
}

void set_countdown_5_and_advance_state_1_alt(Object *object) {
    object->countdown = 5;
    object->state_1++;
    place_at_parent_offset_and_advance_sequence(object);
}

void place_at_parent_offset_and_advance_sequence(Object *object) {
    place_object_at_parent_with_offset_table(object);
    advance_sequence_checked(object);
}

void count_down_reload_23_advance_sequence(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->countdown = 0x17;
        object->state_1++;
    }
    advance_sequence_checked(object);
}

void count_down_then_advance_or_offset(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        if (object->active_state != 0) {
            advance_state_0_reset_state_1(object);
        } else {
            object->state_1++;
            set_sequence_from_table_5(object, 0x14, (SequenceStep **)seqs_c7f8);
        }
    } else if (object->variant_index_2 != 0 || object->variant_index != 0) {
        place_object_at_parent_with_offset_table(object);
    } else {
        advance_sequence_checked(object);
    }
}

void func_801480b0(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_1 = 0;
        object->state_0++;
    } else {
        place_at_parent_offset_and_advance_sequence(object);
    }
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fns_cbd0 holds this function and is declared with this parameter. */
void push_object_to_pool_table_wrapper_8(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fns_cbd0 holds this function and is declared with this parameter. */
void push_object_to_pool_table_wrapper_9(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

void set_position_from_parent(Object *object) {
    Object *parent = object->parent;
    object->facing_side = parent->facing_side;
    if (object->facing_side != 0) {
        object->pos_x = parent->pos_x - object->health;
    } else {
        object->pos_x = parent->pos_x + object->health;
    }
    object->pos_y = parent->pos_y - object->y_offset;
}

void set_position_from_parent_with_velocity(Object *object) {
    Object *parent = object->parent;
    int a = object->vel_x;
    int b;
    object->facing_side = parent->facing_side;
    b = object->vel_y;
    if (object->facing_side != 0) {
        object->pos_x = parent->pos_x + (a >> 16) - object->health;
    } else {
        object->pos_x = parent->pos_x + (a >> 16) + object->health;
    }
    object->pos_y = parent->pos_y - ((b >> 16) + object->y_offset);
    object->vel_x += object->accel_x;
    object->vel_y += object->accel_y;
}
