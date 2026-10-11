/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b18a8_slot04_0b(Object *obj);

void func_801b16f8_slot04_0b(Object *obj) {
    if (obj->vel_x >= 0) {
        func_801b18a8_slot04_0b(obj);
        if (obj->field_0b == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    func_80130efc(obj);
}

void func_801b179c_slot04_0b(Object *obj) {
    int a;

    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    func_801b18a8_slot04_0b(obj);
    a = 0x29;
    if (obj->pos_y >= obj->field_70) {
        obj->state_3++;
        obj->field_45 = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        obj->field_17b = 0;
        if (obj->field_12a != 0) {
            a = 0x2a;
        }
        func_801307e0(obj, a);
    } else {
        func_80130efc(obj);
    }
}
