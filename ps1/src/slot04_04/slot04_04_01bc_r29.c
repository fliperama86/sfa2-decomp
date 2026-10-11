/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4374_slot04_04[];

void func_801b25dc_slot04_04(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_27b = data_801c4374_slot04_04[3];
        } else {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
            obj->field_27b = data_801c4374_slot04_04[obj->field_12a >> 1];
        }
        obj->field_165 = 0;
        func_801204f4(obj, obj->side, 5);
    }
}

void func_801b2688_slot04_04(Object *obj) {
    u16 t;

    if (obj->vel_x != 0) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x - obj->accel_x;
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
        }
    }
    t = obj->step_flags;
    if (t & 0xff) {
        obj->step_flags = t & 0xff00;
        obj->vel_x = obj->vel_y;
        obj->accel_x = obj->accel_y;
    }
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
