/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c64b8_slot04_02[];

void func_801b452c_slot04_02(Object *obj) {
    int k;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3++;
        k = obj->direction_index << 1;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->vel_x = data_801c64b8_slot04_02[(u8)k];
        obj->accel_x = data_801c64b8_slot04_02[(u8)(k | 1)];
        obj->vel_y = data_801c64b8_slot04_02[(u8)(k + 2)];
        obj->accel_y = data_801c64b8_slot04_02[(u8)(k + 3)];
    }
}
