/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1d44_slot04_04(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_45 = 0;
        obj->state_3++;
    } else {
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
        if (obj->field_0b != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
    update_sequence(obj);
}

void func_801b1de8_slot04_04(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->field_45 = 1;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xb);
        obj->vel_x = obj->vel_y;
        obj->accel_x = obj->accel_y;
    }
    update_sequence(obj);
}

void func_801b1e50_slot04_04(Object *obj) {
    if (!((s16)obj->step_flags & 0x8000)) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->vel_y = 0;
        obj->accel_y = 0;
        func_801312b8(obj);
    }
}
