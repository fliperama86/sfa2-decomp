/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b4c_slot04_01(Object *obj);
extern ObjectFn data_801bf128_slot04_01[];

int func_801b388c_slot04_01(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->vel_y < 0;
}

void func_801b38c4_slot04_01(Object *obj) {
    data_801bf128_slot04_01[obj->state_3](obj);
    func_801b3b4c_slot04_01(obj);
}
