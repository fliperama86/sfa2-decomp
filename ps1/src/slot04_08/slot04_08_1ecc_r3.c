/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b20e0_slot04_08(Object *obj) {
    Object *p;
    obj->countdown = (s16)obj->countdown - 1;
    if (obj->countdown & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        obj->facing_side = obj->facing_side ^ 1;
        p = obj->other;
        p->field_15b = 1;
        func_80140770(obj, 2, 0xf, -0x200, 0xf, 0, 0);
        if ((s16)p->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x18;
            }
        }
        if (obj->facing_side != 0) {
            obj->vel_x = -0x18000;
        } else {
            obj->vel_x = 0x18000;
        }
        obj->vel_y = -0x50000;
        obj->accel_y = 0x5000;
        set_sequence_by_side(obj, 0x30);
    }
}

void func_801b21c8_slot04_08(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->ground_y < obj->pos_y) {
        obj->pos_y = obj->ground_y;
        obj->active_state = 0;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac & 0xffff0000;
        func_801209c4(obj);
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}
