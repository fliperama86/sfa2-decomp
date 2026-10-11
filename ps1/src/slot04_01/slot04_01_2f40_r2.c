/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3098_slot04_01(Object *obj) {
    s32 t = obj->vel_y + obj->accel_y;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - t;
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_y = t;
    if (obj->pos_y < obj->field_70) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    }
}
