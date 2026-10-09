/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ca4_slot04_02(Object *obj) {
    s16 t;

    t = obj->field_3a;
    if (t < 0) {
        func_801312b8(obj);
    } else if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        func_80146998(obj);
    } else {
        func_80130efc(obj);
    }
}
