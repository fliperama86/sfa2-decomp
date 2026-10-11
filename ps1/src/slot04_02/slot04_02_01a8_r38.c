/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4d28_slot04_02(Object *obj);

void func_801b4c34_slot04_02(Object *obj) {
    obj->state_3 = 9;
    obj->field_159 = 1;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x52);
}

void func_801b4c6c_slot04_02(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->field_17b = 0;
        obj->step_timer = 1;
        func_801307e0(obj, 0x50);
    } else {
        if ((u8)obj->step_flags == 0) {
            update_sequence(obj);
        }
        func_801b4d28_slot04_02(obj);
    }
}
