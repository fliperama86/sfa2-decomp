/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b28dc_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p;
    o->field_27b = 0x10;
    p = o->other;
    o->field_07++;
    o->field_157 = 0;
    obj->field_334 = p->field_0c;
    obj->field_335 = p->field_0d;
    func_80146998(o);
    func_801307e0(o, 0x28);
}

void func_801b2940_slot04_09(Object *obj) {
    func_80130efc(obj);
    if (obj->field_3a & 1) {
        if (obj->field_241 != 0) {
            obj->field_07 = 6;
            obj->other->field_6b = 0;
            func_801307e0(obj, 0x2a);
        } else {
            obj->field_46 = 8;
            obj->field_4c = 0x98000;
            obj->field_50 = 0;
            obj->field_54 = 0;
            obj->field_58 = 0;
            obj->field_07++;
            obj->field_3a = obj->field_3a & 0xff00;
            if (obj->field_0b == 0) {
                obj->field_4c = 0xfff68000;
            }
        }
    }
}
