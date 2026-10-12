/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3ec8_slot04_04(Object *obj) {
    if (obj->field_3c->frame->field_09 == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}

void func_801b3f00_slot04_04(Object *obj) {
    obj->state_0 = 1;
    obj->field_00 = 1;
    obj->state_1 = 0;
    obj->state_2 = 0;
    obj->state_3 = 0;
    obj->health = 0xff;
}
