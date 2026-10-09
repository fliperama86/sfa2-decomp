/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);
void func_801b4098_slot04_09(Object *obj);

void func_801b4040_slot04_09(Object *obj) {
    obj->field_07++;
    obj->field_50 = 0;
    obj->field_58 = 0;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x30000;
        obj->field_54 = 0x2000;
        func_801b4098_slot04_09(obj);
    } else {
        obj->field_4c = -0x30000;
        obj->field_54 = -0x2000;
        func_801b4098_slot04_09(obj);
    }
}

void func_801b4098_slot04_09(Object *obj) {
    u16 t;
    func_80130efc(obj);
    t = obj->field_3a;
    if ((u8)t == 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_27b = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            obj->field_27b = 3;
        }
    } else if ((u8)t != 2) {
        obj->field_3a = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x31c);
        func_801483a4(obj, -0xc, 0x3f);
    }
}

void func_801b415c_slot04_09(Object *obj) {
    func_801b2858_slot04_09(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        func_801204f4(obj, ((Slot04aObj *)obj)->field_a6, 4);
    }
    func_80130efc(obj);
}
