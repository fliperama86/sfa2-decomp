/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b08c4_slot04_02(Object *obj);

void func_801b0878_slot04_02(Object *obj) {
    obj->vel_y = -0x4c000;
    obj->state_3 = obj->state_3 + 1;
    if (obj->field_0b != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    func_801b08c4_slot04_02(obj);
}

void func_801b08c4_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    if (obj->ground_y <= (u16)obj->pos_y) {
        func_801209c4(obj);
        obj->pos_y = (u16)obj->ground_y;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
