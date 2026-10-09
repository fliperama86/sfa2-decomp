/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3ec8_slot04_04(Object *obj) {
    if (obj->field_3c->frame->field_09 == 0) {
        obj->field_04 = obj->field_04 + 1;
    }
}

void func_801b3f00_slot04_04(Object *obj) {
    obj->field_04 = 1;
    obj->field_00 = 1;
    obj->field_05 = 0;
    obj->field_06 = 0;
    obj->field_07 = 0;
    obj->field_5c = 0xff;
}
