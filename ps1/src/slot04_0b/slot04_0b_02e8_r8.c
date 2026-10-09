/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0f80_slot04_0b(Object *obj);

int func_801b0f30_slot04_0b(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b0f80_slot04_0b(obj);
    }
    return r;
}
