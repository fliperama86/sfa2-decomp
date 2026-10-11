/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013cc4_slot27(Object *obj);

void func_8001259c_slot27(Object *obj) {
    if (!(obj->field_22 & 0x8000)) {
        func_80013cc4_slot27(obj);
    } else {
        obj->state_2 = 0;
        obj->state_1 = obj->state_1 + 1;
        obj->field_3c->field_00 = 2;
    }
}
