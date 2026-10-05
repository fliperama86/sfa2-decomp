/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80143d68(Object *object) {
    object->pos_x -= 9;
    object->field_5c = 0;
    object->pos_y = 0x70;
    if (object->pos_x < 0xc1) {
        object->field_05 = object->field_05 + 1;
        if (object->field_a0 == 1) func_80144f10(2, 0);
    }
}
