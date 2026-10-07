/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b340c_slot04_00(Object *o) {
    if ((s16)o->field_3a >= 0) {
        func_80131094(o);
    } else {
        o->field_04 = 3;
        o->field_05 = 0;
        o->field_06 = 0;
        o->field_07 = 0;
    }
}
