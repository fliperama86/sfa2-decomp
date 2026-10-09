/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9248_slot06_0a(Object *obj, int a, int b) {
    int t;

    /* The steps reuse the two parameters and one local. Written as two expressions eleven instruction slots differ. */
    a = a * 3 >> 3;
    t = b >> 3;
    a += obj->field_4c;
    *(s32 *)&obj->field_10 = a;
    a = b >> 2;
    b = t + a;
    b += obj->field_50;
    *(s32 *)&obj->field_14 = 0xf80000 - b;
}
