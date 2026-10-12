/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1454_slot04_05[];

void func_801b1718_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y = obj->accel_y - data_801c1454_slot04_05[obj->field_12a >> 1];
    }
    if ((u8)func_80130184(obj) != 0) {
        if (obj->step_flags & 0x80) {
            return;
        }
    } else {
        obj->state_3++;
        func_801209c4(obj);
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->ground_y;
    }
    update_sequence(obj);
}
