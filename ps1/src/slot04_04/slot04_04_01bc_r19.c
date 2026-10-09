/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1a08_slot04_04(Object *obj);

void func_801b19bc_slot04_04(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 != 0) {
        func_80130efc(obj);
    } else {
        func_801b1a08_slot04_04(obj);
    }
}

void func_801b1a08_slot04_04(Object *obj) {
    u16 a;

    obj->field_07++;
    a = 0x21;
    obj->field_17b = 0;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    func_801307e0(obj, a + obj->field_12a + 1);
}
