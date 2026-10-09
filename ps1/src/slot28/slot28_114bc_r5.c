/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80022098_slot28(Object *obj) {
    *(int *)&obj->field_14 += 0x8000;
    if (obj->pos_y >= 0xc0) {
        obj->pos_y = 0xc0;
        obj->field_05++;
    }
}
