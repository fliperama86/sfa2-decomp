/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b223c_slot04_03(Object *obj);

void func_801b2148_slot04_03(Object *obj) {
    s16 y;

    if (*(u8 *)&obj->step_flags != 0) {
        update_sequence(obj);
        return;
    }
    obj->field_45 = 1;
    func_801b223c_slot04_03(obj);
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    y = obj->ground_y;
    if (y < obj->pos_y) {
        obj->pos_y = y;
        obj->field_45 = 0;
        func_801209c4(obj);
        obj->state_3++;
        func_801307e0(obj, 0x28);
    } else {
        update_sequence(obj);
    }
}
