/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_scratch_function_by_side(Object *object) {
    if (object->side == 0) {
        scratch_call_5c(object);
    } else {
        scratch_call_10c(object);
    }
}

void call_handler_by_kind_and_side(Object *object) {
    if (object->kind == 2 || object->kind == 6 || object->kind == 0xc ||
        object->kind == 0xe || object->kind == 0x11 ||
        (u8)(object->kind - 0x13) < 2) {
        if (object->side == 0) {
            scratch_call_98(object);
        } else {
            scratch_call_148(object);
        }
    } else {
        call_handler_by_state_2(object);
    }
}

void call_handler_by_state_2(Object *object) {
    handlers_1a80[object->state_2](object);
}

void advance_state_2_and_restore_facing(Object *object) {
    object->state_2 = object->state_2 + 1;
    object->facing_side = object->saved_facing;
    set_sequence_by_side(object, 0);
}

void advance_state_2_unless_hold_flag(Object *object) {
    if (game_state.config->hold_flag == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void start_countdown_and_set_sequence(Object *object) {
    int arg = 0x28;

    object->countdown = 0x78;
    object->state_2 = object->state_2 + 1;
    if (game_state.config->table_index != 0) {
        arg = 0x29;
    }
    set_sequence_by_side(object, arg);
}
