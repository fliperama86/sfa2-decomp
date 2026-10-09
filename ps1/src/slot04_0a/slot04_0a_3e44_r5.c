/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_801c0854_slot04_0a[];

void func_801b4334_slot04_0a(Object *obj) {
    int v = data_801c0854_slot04_0a[obj->field_ac >> 1];

    if (obj->field_0b == 0) {
        obj->field_4c = -v;
    } else {
        obj->field_4c = v;
    }
}

