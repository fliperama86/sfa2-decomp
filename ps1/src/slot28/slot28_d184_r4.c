/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001d984_slot28(Object *obj) {
    *(int *)&obj->pos_x_frac = *(int *)&obj->pos_x_frac - 0x80000;
    if (obj->pos_x < 0xe1) {
        obj->pos_x = 0xe8;
        obj->state_1 = obj->state_1 + 1;
    }
}
