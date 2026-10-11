/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c260c_slot04_0b[];

void func_801b28c0_slot04_0b(Object *obj) {
    int k;

    if ((u8)obj->step_flags == 6) {
        obj->state_3++;
        k = (obj->field_12a >> 1) * 3 + obj->field_a0;
        obj->vel_x = data_801c260c_slot04_0b[k];
        obj->vel_y = data_801c260c_slot04_0b[k + 1];
        obj->accel_x = -0x8000;
        obj->accel_y = data_801c260c_slot04_0b[k + 2];
        obj->field_45 = 1;
    }
    func_80130efc(obj);
}

void func_801b296c_slot04_0b(Object *obj) {
    if (obj->vel_x >= 0) {
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->field_0b == 0) {
            *(s32 *)&obj->pos_x_frac -= obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        obj->vel_x += obj->accel_x;
    } else {
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->state_3++;
    }
    func_80130efc(obj);
}
