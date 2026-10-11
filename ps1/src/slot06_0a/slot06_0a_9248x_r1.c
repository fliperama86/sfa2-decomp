/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9248_slot06_0a(Object *obj, int a, int b) {
    int t;

    /* The steps reuse the two parameters and one local. Written as two expressions eleven instruction slots differ. */
    a = a * 3 >> 3;
    t = b >> 3;
    a += obj->vel_x;
    *(s32 *)&obj->pos_x_frac = a;
    a = b >> 2;
    b = t + a;
    b += obj->vel_y;
    *(s32 *)&obj->pos_y_frac = 0xf80000 - b;
}
