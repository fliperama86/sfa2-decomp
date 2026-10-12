/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec42274 data_80042274_slot28[];

void func_8002026c_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];

    obj->state_0++;
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->vel_x = data_80042274_slot28[obj->field_03].field_00;
    obj->vel_y = data_80042274_slot28[obj->field_03].field_04;
    obj->accel_x = data_80042274_slot28[obj->field_03].field_08;
    obj->accel_y = data_80042274_slot28[obj->field_03].field_0c;
    obj->field_0c = 0xff;
    if (obj->field_03 != 0) {
        obj->field_0d = 0;
        obj->field_0b = get_random_byte() & 1;
        func_80130768(obj, 2, (SequenceStep **)obj->box_tables);
    } else {
        obj->field_0b = 1;
        obj->field_0d = 0;
        func_80130768(obj, 0, (SequenceStep **)obj->box_tables);
    }
}
