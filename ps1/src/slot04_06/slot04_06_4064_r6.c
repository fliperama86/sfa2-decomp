/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj) {
    int t = obj->vel_x;

    if (obj->field_0b == 0) {
        t = -t;
    }
    *(s32 *)&obj->pos_x_frac += t;
    obj->vel_x += obj->accel_x;
}

void func_801b4850_slot04_06(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void func_801b4874_slot04_06(Object *obj, u16 *p) {
    if (obj->field_0b == 0) {
        *p = -*p;
    }
}
