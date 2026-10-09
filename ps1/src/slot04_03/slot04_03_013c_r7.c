/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0fd4_slot04_03(Object *obj);

int func_801b0f84_slot04_03(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b0fd4_slot04_03(obj);
    }
    return r;
}
