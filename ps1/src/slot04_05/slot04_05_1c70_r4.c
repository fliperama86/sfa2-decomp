/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2068_slot04_05(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_46 = 0x1e;
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b20a8_slot04_05(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_801307e0(obj, 0x26);
    }
}
