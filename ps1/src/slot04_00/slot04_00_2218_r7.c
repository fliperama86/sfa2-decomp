/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *object);

void func_801b291c_slot04_00(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_07++;
        func_801307e0(obj, 0x3a);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    }
}
