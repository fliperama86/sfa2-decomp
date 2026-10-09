/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2b84_slot04_06(Object *obj);

void func_801b2a88_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;
    Object *other;

    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else if (obj->field_12a == 4) {
        func_801b2b84_slot04_06(obj);
    } else {
        other = obj->other;
        other->field_15b = 1;
        other->field_260 = 1;
        func_80140770(obj, 2, 0xf, -0x200, 0, 0, 0);
        if ((s16)other->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
        }
        obj->field_4c = -0x20000;
        obj->field_50 = 0x44000;
        obj->field_54 = 0;
        obj->field_58 = -0x4800;
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        func_801307e0(obj, 0x26);
    }
}

void func_801b2b84_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;

    obj->field_07 = 0x13;
    obj->field_4c = 0x80000;
    obj->field_54 = 0;
    func_80120554((Object *)&obj->other, s->field_e6, 0x34e);
    func_801307e0(obj, 0x38);
}
