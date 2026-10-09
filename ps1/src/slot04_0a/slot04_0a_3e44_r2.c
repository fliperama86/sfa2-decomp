/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3f58_slot04_0a(Object *obj) {
    int i;
    u32 z = 0;
    u32 *p = (u32 *)obj->slots;

    for (i = 0x13; i >= 0; i--) {
        *p++ = z;
    }
}

