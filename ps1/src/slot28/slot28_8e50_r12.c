/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80019960_slot28(Object *obj) {
    *(s32 *)&obj->field_10 -= 0x4000;
    if (obj->pos_x <= 0xc8) {
        obj->pos_x = 0xc8;
        obj->pos_y = 0xa0;
        obj->field_05++;
    }
}
