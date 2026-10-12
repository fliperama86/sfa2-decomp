/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8013d0c8(Object *object) {
    unsigned v = (object->pressed_bits | object->released_bits) & 0x95;
    int r;
    if (v == 0x94) {
        r = 1;
    } else {
        r = v == 1;
    }
    return r;
}
