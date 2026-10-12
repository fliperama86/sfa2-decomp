/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80144dcc(Object *object) {
    int v;
    update_position_x_with_accel(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        advance_state_0_2(object);
    }
}
