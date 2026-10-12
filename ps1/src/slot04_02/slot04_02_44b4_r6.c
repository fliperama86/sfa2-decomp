/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4d28_slot04_02(Object *obj);

void func_801b5184_slot04_02(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3 = obj->state_3 + 1;
        func_801307e0(obj, 0x56);
        func_80142adc(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
        func_801b4d28_slot04_02(obj);
    }
}

void func_801b5234_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->field_157 = 0;
        obj->active_state = 0;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}
