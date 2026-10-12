/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void move_x_until_countdown_then_advance_state_0_2(Object *object) {
    int v;
    update_position_x_with_accel(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        advance_state_0_2(object);
    }
}
