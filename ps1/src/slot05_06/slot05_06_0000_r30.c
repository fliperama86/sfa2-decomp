/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
void func_801cc84c_slot05_06(Object *obj);

void func_801ca654_slot05_06(Object *obj) {

    func_801cc84c_slot05_06(obj);
    if (obj->field_50 <= 0x1ffff && ((Slot04aObj *)obj)->field_1c8 == 0) {
        ((Slot04aObj *)obj)->field_1c8 = 1;
        func_801307e0(obj, 0x29);
    } else if (obj->field_50 >= 0) {
        func_801cc814_slot05_06(obj);
        func_80130efc(obj);
    } else {
        ((Slot04aObj *)obj)->field_1c2 = 10;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_12a == 0) {
            obj->field_07 = 0xf;
            obj->field_4c = 0;
            obj->field_54 = 0;
            func_801204f4(obj, obj->side, 5);
            func_801307e0(obj, 0x24);
        }
    }
}

void func_801ca724_slot05_06(Object *obj) {
    s32 a;

    ((Slot04aObj *)obj)->field_1c2 = ((Slot04aObj *)obj)->field_1c2 - 1;
    if (((Slot04aObj *)obj)->field_1c2 & 0x80) {
        obj->field_07 = obj->field_07 + 1;
        ((Slot04aObj *)obj)->field_1c2 = 0x28;
    }
    a = 0x4000;
    if (obj->field_0b == 0) {
        a = -0x4000;
    }
    *(s32 *)&obj->field_10 += a;
    *(s32 *)&obj->field_14 += -0x10000;
    func_80130efc(obj);
}
