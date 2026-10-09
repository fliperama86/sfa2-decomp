/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b59c8_slot04_09(Object *obj);

void func_801b06a8_slot04_09(Object *obj) {
    u16 t = obj->field_3a;

    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
    }
    t = obj->field_3a;
    if ((t & 0xff) == 2) {
        obj->field_3a = t & 0xff00;
        func_801b59c8_slot04_09(obj);
    }
    func_80130efc(obj);
}
