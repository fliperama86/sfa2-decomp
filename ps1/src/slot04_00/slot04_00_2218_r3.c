/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1c5c_slot04_00(Object *obj);

void func_801b25f0_slot04_00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        if (--((Slot04aObj *)obj)->field_1a4 == 0) {
            obj->field_07++;
            func_801307e0(obj, (obj->field_12a >> 1) + 0x35);
            return;
        }
    }
    *(s32 *)&obj->field_14 += 0x2000;
    func_80130efc(obj);
}

void func_801b2670_slot04_00(Object *obj) {
    if (func_801b1c5c_slot04_00(obj) != 0) {
        obj->field_07++;
        func_801209c4(obj);
        func_801307e0(obj, 0x30);
    } else {
        func_80130efc(obj);
    }
}
