/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144734(Object *object) {
    update_position_x_with_accel(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        advance_state_0_and_clear_config_mode(object);
    }
}

void call_state_1_handler_3(Object *object) {
    handlers_b630[object->state_1](object);
}

void call_handler_by_state_2_b640_and_advance(Object *object) {
    handlers_b640[object->state_2](object);
    advance_sequence_checked(object);
    if (object->state_2 >= 2) {
        set_flag_and_push_if_mode_8(object);
    }
}
