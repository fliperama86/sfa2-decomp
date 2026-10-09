/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b5eb8_slot04_02(Object *obj);
extern u16 box_margin[];

void func_801b5e4c_slot04_02(Object *obj) {
    if (func_801b5eb8_slot04_02(obj)) {
        if (obj->field_70 <= obj->pos_y) {
            obj->pos_y = obj->field_70;
            game_state.field_63 = 0x18;
            obj->field_07 = obj->field_07 + 1;
        }
    }
}

u8 func_801b5eb8_slot04_02(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    return obj->field_50 < 0;
}

u8 func_801b5ef0_slot04_02(Object *obj) {
    return (s16)(box_margin[0] + 0xc0) < obj->pos_x;
}
