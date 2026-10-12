/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 get_pressed_bit_8(Object *object) {
    return *((u8 *)&object->pressed_bits + 1) & 1;
}
