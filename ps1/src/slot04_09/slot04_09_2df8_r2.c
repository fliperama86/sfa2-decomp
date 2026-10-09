/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);

void func_801b2ef8_slot04_09(Object *obj) {
    u16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if ((u8)t == 0) {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_27b = 0;
        if (obj->field_165 & 0x80) {
            obj->other->field_6b = 10;
            obj->field_27b = 5;
        }
        obj->field_165 = 0;
        obj->field_4c = 0x80000;
        obj->field_54 = -0x8000;
        obj->field_50 = 0x80000;
        obj->field_58 = -0x6000;
        if (obj->field_0b == 0) {
            obj->field_4c = -0x80000;
            obj->field_54 = 0x8000;
        }
        func_801307e0(obj, 0x33);
    } else if ((u8)t != 2) {
        obj->field_3a = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x1f, 0x2e);
    }
}

void func_801b3000_slot04_09(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_801b27f8_slot04_09(obj);
    func_80130efc(obj);
}
