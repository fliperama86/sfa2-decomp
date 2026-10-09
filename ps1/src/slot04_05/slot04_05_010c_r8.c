/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b12c0_slot04_05(Object *obj);

int func_801b1270_slot04_05(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b12c0_slot04_05(obj);
    }
    return r;
}
