/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80137c88(Object *object) {
    int v = object->field_46;
    v -= 0x100;
    object->field_46 = v;
    if ((v & 0xff00) == 0) {
        object->field_00 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}
