/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3d8c_slot04_07(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0, 0, 1, 0);
    }
    func_80130efc(obj);
}

void func_801b3df0_slot04_07(Object *obj) {
    func_80140770(obj, 0, 5, 0, 0, 0, 1);
    obj->field_07 = obj->field_07 + 1;
    func_80130efc(obj);
}
