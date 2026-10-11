/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4608_slot04_02(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->pos_y = obj->pos_y - 0x28;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->field_46 = (u8)obj->field_46 | 0xa00;
    }
    if (obj->field_0b != 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac -= obj->vel_x;
    }
    obj->vel_x += obj->accel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}
