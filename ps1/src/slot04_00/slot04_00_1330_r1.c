/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bfe08_slot04_00[];

void func_801b1330_slot04_00(Object *obj) {
    int a = 0x1b;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    obj->accel_x = -0x8000;
    obj->accel_y = -0x6000;
    obj->vel_x = data_801bfe08_slot04_00[obj->direction_index];
    obj->vel_y = data_801bfe08_slot04_00[obj->direction_index + 1];
    if (obj->field_49 != 0) {
        a = 0x3b;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + a);
}
