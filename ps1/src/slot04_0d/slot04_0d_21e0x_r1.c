/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b21e0_slot04_0d(Object *obj) {
    s32 a;

    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->field_70 <= obj->pos_y) {
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->state_3++;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x34);
    } else {
        if (obj->field_0b != 0) {
            a = obj->vel_x;
        } else {
            a = -obj->vel_x;
        }
        *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
        func_80130efc(obj);
    }
}
