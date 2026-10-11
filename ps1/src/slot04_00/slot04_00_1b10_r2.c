/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bfe4c_slot04_00[];

int func_801b1c5c_slot04_00(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return !(obj->pos_y < obj->field_70);
}

void func_801b1ca0_slot04_00(Object *obj) {
    data_801bfe4c_slot04_00[obj->state_3](obj);
    if (((Slot04aObj *)obj)->field_1a5 != 0 && obj->vel_y < 0 && obj->pos_y - obj->field_70 >= -0xf) {
        ((Slot04aObj *)obj)->field_1a5 = 0;
    }
}
