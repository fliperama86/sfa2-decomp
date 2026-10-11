/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b567c_slot04_02(Object *obj) {
    u16 t;
    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        func_80120554(obj, obj->side, 0x320);
    }
    if (obj->field_0b != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= obj->field_70) {
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x5d);
    }
}
