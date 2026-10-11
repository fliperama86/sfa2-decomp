/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1424_slot04_05[];

void func_801b15ac_slot04_05(Object *obj) {
    int a = 0x1c;

    obj->state_3++;
    obj->field_17b = 1;
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    obj->vel_x = data_801c1424_slot04_05[obj->field_12a * 2];
    obj->vel_y = data_801c1424_slot04_05[obj->field_12a * 2 + 1];
    obj->accel_x = data_801c1424_slot04_05[obj->field_12a * 2 + 2];
    obj->accel_y = data_801c1424_slot04_05[obj->field_12a * 2 + 3];
    if (obj->field_0b == 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    if (obj->field_49 != 0) {
        a = 0x38;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}

void func_801b16d0_slot04_05(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_45 = 1;
        obj->state_3++;
    }
}
