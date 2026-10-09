/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
void func_801cc84c_slot05_06(Object *obj);

void func_801c983c_slot05_06(Object *o) {
    if (*(u8 *)&o->field_3a != 0) {
        o->field_45 = 1;
        ((Slot04aObj *)o)->field_1c8 = 0;
        o->field_07++;
    }
    func_80130efc(o);
}

void func_801c9880_slot05_06(Object *o) {
    func_801cc84c_slot05_06(o);
    if (o->field_50 <= 0x1ffff && ((Slot04aObj *)o)->field_1c8 == 0) {
        ((Slot04aObj *)o)->field_1c8 = 1;
        func_801307e0(o, 0x28);
    } else if (o->field_50 >= 0) {
        func_801cc814_slot05_06(o);
        func_80130efc(o);
    } else {
        o->field_4c = 0x18000;
        o->field_50 = -0x80000;
        o->field_07++;
        func_801204f4(o, o->side, 4);
        func_801307e0(o, 0x20);
    }
}
