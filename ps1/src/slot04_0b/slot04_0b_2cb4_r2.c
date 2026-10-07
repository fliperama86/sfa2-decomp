/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ebc_slot04_0b(Object *obj) {
    obj->field_4c = 0x98000;
    obj->field_50 = 0x90000;
    obj->field_54 = -0x8000;
    obj->field_58 = -0x6000;
    obj->field_07++;
}

void func_801b2ef0_slot04_0b(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0xff00) {
        obj->field_3a = t & 0xff;
        func_801204f4(obj, obj->side, 0xf);
        func_801204f4(obj, obj->side, 4);
    }
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_07++;
    }
    func_80130efc(obj);
}
