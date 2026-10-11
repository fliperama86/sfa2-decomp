/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2568_slot04_0b[];

void func_801b15d0_slot04_0b(Object *obj) {
    int a;

    a = 0x26;
    obj->field_17b = 1;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 0xf);
    func_801204f4(obj, obj->side, 5);
    obj->accel_x = -0x8000;
    obj->accel_y = -0x6000;
    obj->vel_x = data_801c2568_slot04_0b[obj->field_12a];
    obj->vel_y = data_801c2568_slot04_0b[obj->field_12a + 1];
    if (obj->field_49 != 0) {
        a = 0x33;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
