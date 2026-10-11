/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);

void func_801b183c_slot04_06(Object *o) {
    if (*(u8 *)&o->step_flags != 0) {
        o->field_45 = 1;
        ((Slot04aObj *)o)->field_1c8 = 0;
        o->state_3++;
    }
    update_sequence(o);
}

void func_801b1880_slot04_06(Object *o) {
    func_801b4850_slot04_06(o);
    if (o->vel_y <= 0x1ffff && ((Slot04aObj *)o)->field_1c8 == 0) {
        ((Slot04aObj *)o)->field_1c8 = 1;
        func_801307e0(o, 0x28);
    } else if (o->vel_y >= 0) {
        func_801b4818_slot04_06(o);
        update_sequence(o);
    } else {
        o->vel_x = 0x18000;
        o->vel_y = -0x80000;
        o->state_3++;
        func_801204f4(o, o->side, 4);
        func_801307e0(o, 0x20);
    }
}
