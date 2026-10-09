/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0f44_slot04_03(Object *obj) {
    u8 i;
    u8 *a = (u8 *)obj->slots;
    u8 *b = (u8 *)&obj->slots[2];
    u8 *c = (u8 *)&obj->slots[5];
    for (i = 0; i < 8; i++) {
        *a++ = 0;
        *b++ = 0;
        *c++ = 0;
    }
}
