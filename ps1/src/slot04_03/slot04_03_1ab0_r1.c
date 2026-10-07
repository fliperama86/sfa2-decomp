/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1ab0_slot04_03(Object *obj) {
    u16 t = obj->field_3a;

    if ((t & 0xff) != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        if (obj->field_0b != 0) {
            obj->field_4c = 0x50000;
            obj->field_54 = -0x2000;
        } else {
            obj->field_4c = -0x50000;
            obj->field_54 = 0x2000;
        }
    } else if ((t & 0xff00) == 0) {
        *(s32 *)&obj->field_10 += obj->field_4c;
    }
    func_80130efc(obj);
}

void func_801b1b4c_slot04_03(Object *obj) {
    if ((u8)obj->field_3a != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
    }
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    func_80130efc(obj);
}

void func_801b1bb8_slot04_03(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
    }
}
