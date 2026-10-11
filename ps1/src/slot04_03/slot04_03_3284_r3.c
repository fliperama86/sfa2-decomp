/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3530_slot04_03(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->field_70 > obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->field_70;
        game_state.field_63 = 0x30;
        func_80146478(obj, 2, 0, 0x40);
        if ((func_80140cd8(obj, 0x10, 0) & 0xff) != 0) {
            ref_other.p = obj->other;
            func_80120554(ref_other.p, ref_other.p->side, 0x336);
        } else {
            ref_other.p = obj->other;
            func_80120554(ref_other.p, ref_other.p->side, 0x304);
        }
        func_801307e0(obj, 0x1a);
    }
}

void func_801b3664_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0, 0xa, -0x200, 0, 0, 0);
    }
    func_80130efc(obj);
}
