/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1548_slot04_07(Object *obj);

int func_801b14f8_slot04_07(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b1548_slot04_07(obj);
    }
    return r;
}
