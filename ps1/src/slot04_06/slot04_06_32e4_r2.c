/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c5478_slot04_06[];

void func_801b346c_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        obj->state_3++;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x45);
    } else {
        obj->state_3 = 0;
    }
}

void func_801b34bc_slot04_06(Object *obj) {
    Object *other;
    obj->state_3++;
    other = obj->other;
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    other->state_0 = 1;
    other->state_1 = 3;
    other->state_2 = 0;
    other->state_3 = 0;
    other->field_73 = 0xff;
    other->field_15b = 1;
    other->gauge = 0x64;
    obj->vel_x = 0xa0000;
    obj->accel_x = -0xe000;
    obj->field_73 = 1;
    obj->gauge = 0x80;
    ((Slot04aObj *)obj)->field_1c5 = data_801c5478_slot04_06[obj->field_12a >> 1];
    func_801307e0(obj, 0x2c);
}
