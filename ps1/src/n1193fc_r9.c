/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 box_margin;

void func_8011ffa8(Object *object) {
    int m = box_margin;
    if ((u16)(object->pos_x - m + 0x80) > 0x280) {
        object->field_04 = 3;
    }
}
