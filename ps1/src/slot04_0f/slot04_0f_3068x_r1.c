/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3068_slot04_0f(Object *obj) {
    if (obj->vel_y >= 0) {
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->field_70 <= obj->pos_y) {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
            obj->vel_x += obj->accel_x;
            return;
        }
    }
    obj->state_3++;
    obj->field_259 = 0;
    obj->field_258 = 0;
    obj->field_25a = 0;
    func_801204f4(obj, obj->side, 0xe);
    func_801307e0(obj, 0x47);
}
