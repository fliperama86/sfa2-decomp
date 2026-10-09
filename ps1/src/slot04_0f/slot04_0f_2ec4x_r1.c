/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ec4_slot04_0f(Object *obj);

void func_801b2ec4_slot04_0f(Object *obj) {
    int z = 0;
    int i;
    u8 *p;
    u8 c;
    u8 d;

    p = (u8 *)obj + 0x2b0;
    for (c = z, i = 0x3f; i >= 0; i--) {
        *p++ = c;
    }
    p = (u8 *)obj + 0x184;
    for (d = z, i = 0x17; i >= 0; i--) {
        *p++ = d;
    }
}
