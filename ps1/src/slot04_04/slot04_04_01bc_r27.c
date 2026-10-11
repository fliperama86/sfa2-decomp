/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

s32 func_801b3a84_slot04_04(Object *object);

void func_801b230c_slot04_04(Object *obj) {
    if (func_801b3a84_slot04_04(obj) >= 0) {
        update_sequence(obj);
    } else {
        if (obj->field_49 != 0) {
            obj->accel_y = 0xffff0000;
        } else {
            obj->accel_y = -0x6000;
        }
        if (obj->pos_y < obj->field_70) {
            update_sequence(obj);
        } else {
            obj->state_3++;
            obj->pos_y_frac = 0;
            obj->field_45 = 0;
            obj->field_159 = 0;
            obj->field_17b = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, obj->field_12a + 0x31);
        }
    }
}

void func_801b23b4_slot04_04(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}
