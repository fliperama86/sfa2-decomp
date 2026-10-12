/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144320(Object *object) {
    int t;
    *(int *)&object->pos_x_frac = *(int *)&object->pos_x_frac + object->vel_x;
    t = object->countdown - 1;
    object->countdown = t;
    if ((t & 0x80) != 0) {
        object->pos_x = 0xc0;
        object->state_2 = object->state_2 + 1;
    }
}
