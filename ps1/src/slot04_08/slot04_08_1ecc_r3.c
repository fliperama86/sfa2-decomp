/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b20e0_slot04_08(Object *obj) {
    Object *p;
    obj->field_46 = (s16)obj->field_46 - 1;
    if (obj->field_46 & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        obj->field_0b = obj->field_0b ^ 1;
        p = obj->other;
        p->field_15b = 1;
        func_80140770(obj, 2, 0xf, -0x200, 0xf, 0, 0);
        if ((s16)p->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x18;
            }
        }
        if (obj->field_0b != 0) {
            obj->vel_x = -0x18000;
        } else {
            obj->vel_x = 0x18000;
        }
        obj->vel_y = -0x50000;
        obj->accel_y = 0x5000;
        func_80130678(obj, 0x30);
    }
}

void func_801b21c8_slot04_08(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->field_70 < obj->pos_y) {
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac & 0xffff0000;
        func_801209c4(obj);
        obj->state_3 = obj->state_3 + 1;
        func_80130678(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}
