/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c7c98_slot04_09[];

void func_801b1d40_slot04_09(Object *obj) {
    int n = 0x1f;

    obj->field_a2 = 0;
    obj->state_3++;
    if (obj->field_49 != 0) {
        obj->state_3 = 9;
        obj->field_17b = 1;
        obj->field_225 = 1;
    }
    if (obj->field_246 == 0) {
        func_80141f28(obj, 5);
    }
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        n = 0x46;
    }
    func_801307e0(obj, n);
}

void func_801b1de0_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->field_45 = 1;
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        obj->vel_x = data_801c7c98_slot04_09[obj->field_12a];
        obj->vel_y = data_801c7c98_slot04_09[obj->field_12a + 1];
        if (obj->field_0b == 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        func_801307e0(obj, 0x20);
    } else {
        func_80130efc(obj);
    }
}
