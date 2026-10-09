/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b31b0_slot04_05(Object *obj) {
    u16 t = obj->field_3a;
    if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        obj->field_07++;
        func_80146998(obj);
        obj->other->field_6b = 0x14;
    }
    func_80130efc(obj);
}
