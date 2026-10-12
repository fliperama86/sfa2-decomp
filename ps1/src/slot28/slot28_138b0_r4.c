/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec4aac4 data_8004aac4_slot28[];
extern u8 data_8004aab8_slot28[];
extern u8 data_8004aac0_slot28[];

void func_80023dac_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->vel_x = data_8004aac4_slot28[obj->field_03].vel_x;
    obj->accel_x = data_8004aac4_slot28[obj->field_03].accel_x;
    obj->vel_y = data_8004aac4_slot28[obj->field_03].vel_y;
    obj->accel_y = data_8004aac4_slot28[obj->field_03].accel_y;
    if (obj->field_03 != 0) {
        func_80130768(obj, data_8004aab8_slot28[obj->field_03], ((Slot28Obj *)obj)->field_6c);
    } else {
        func_80130768(obj, data_8004aac0_slot28[obj->field_a0], ((Slot28Obj *)obj)->field_6c);
    }
}
