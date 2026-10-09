/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *object);

void func_801b2b74_slot04_01(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_07++;
        if (obj->field_0b == 0) {
            obj->pos_x -= 0x18;
        } else {
            obj->pos_x += 0x18;
        }
        func_801307e0(obj, 0x20);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    }
}
