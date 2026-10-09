/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b30a0_slot04_0a(Object *obj);

void func_801b3054_slot04_0a(Object *obj) {
    if (func_801b1e04_slot04_0a(obj)) {
        func_801b30a0_slot04_0a(obj);
    } else {
        func_80130efc(obj);
    }
}
