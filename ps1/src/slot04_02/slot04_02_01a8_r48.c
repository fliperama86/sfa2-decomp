/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b5eb8_slot04_02(Object *obj);

void func_801b5e4c_slot04_02(Object *obj) {
    if (func_801b5eb8_slot04_02(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.field_63 = 0x18;
            obj->state_3 = obj->state_3 + 1;
        }
    }
}

u8 func_801b5eb8_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->vel_y < 0;
}

u8 func_801b5ef0_slot04_02(Object *obj) {
    return (s16)(box_margin[0] + 0xc0) < obj->pos_x;
}
