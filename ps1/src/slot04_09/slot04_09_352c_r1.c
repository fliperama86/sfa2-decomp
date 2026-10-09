/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3590_slot04_09(Object *obj);

void func_801b352c_slot04_09(Object *obj) {
    obj->field_07++;
    *(s32 *)&obj->field_50 = 0;
    *(s32 *)&obj->field_58 = 0;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x40000;
        obj->field_54 = 0xa000;
    } else {
        obj->field_4c = -0x40000;
        obj->field_54 = 0xffff6000;
    }
    func_801b3590_slot04_09(obj);
}

void func_801b3590_slot04_09(Object *obj) {
    u16 t;
    u8 b;
    func_80130efc(obj);
    t = obj->field_3a;
    b = t;
    if (b == 0) {
        obj->field_07++;
        obj->field_27b = 0;
        obj->field_165 = 0;
        obj->field_3a = obj->field_3a & 0xff00;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            obj->field_27b = 0xc;
        }
    } else if (b != 2) {
        obj->field_3a = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x2c, 0x40);
    }
}
