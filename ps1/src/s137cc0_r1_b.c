/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: advance_state_2_flip_facing_side (original stores countdown through a0 in the delay slot). */
#include "game.h"
#include "externs.h"
#include "protos.h"



void tick_countdown_then_reverse_x_motion(Object *object) {
    *(s16 *)&object->countdown -= 0x100;
    if ((object->countdown & 0xff00) == 0) {
        object->active_count = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
        object->vel_y = 0;
        object->vel_x = -object->vel_x;
        object->accel_x = -object->accel_x;
    }
    advance_sequence_checked(object);
}

void call_handler_2694_by_state_2(Object *object) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        handlers_2694[object->state_2](object);
    }
    func_8011ffdc(object);
}

void flip_facing_set_variant_and_advance(Object *object) {
    object->sequence_changed_flag = 1;
    object->state_2++;
    object->facing_side ^= 1;
    call_kind_9_handler_for_player(object);
    object->countdown = (u8)object->countdown | 0x200;
    if ((object->variant_index_2 & 0x80) == 0) {
        func_80138070(object, object->variant_index_2);
    }
    advance_sequence_checked(object);
}

void tick_countdown_and_reverse_x_motion(Object *object) {
    *(s16 *)&object->countdown -= 0x100;
    if ((s16)object->countdown < 0) {
        object->active_count = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
        object->vel_x = -object->vel_x;
        object->accel_x = -object->accel_x;
        object->vel_y = table_269c[object->vel_table_index >> 1];
    }
    advance_sequence_checked(object);
}

void func_80138070(Object *object, u8 index) {
    if (object->sound_side == 0) {
        object->sequence = seqs_a4_left[index];
    } else {
        object->sequence = seqs_154_right[index];
    }
    object->step_timer = object->sequence->duration;
    object->step_flags = object->sequence->flags;
    object->sequence_changed_flag = 1;
    object->frame = object->frames + object->sequence->frame_index;
}

void call_kind_9_handler_for_player(Object *object) {
    if (player_left.kind == 9) {
        func_80078e44(object);
    } else if (player_right.kind == 9) {
        func_8008ef44(object);
    }
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The tables table_801726bc, table_80172980 and table_801729e4 hold this function and are declared with this parameter. */
void func_80138144(Block172 *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}
