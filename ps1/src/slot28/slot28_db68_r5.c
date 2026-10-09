/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8003dc00_slot28[];

void func_8001e2a0_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x150; i++) {
        ((u16 *)data_801a2fe4)[i] = data_8003dc00_slot28[i];
        ((u16 *)data_801a2fe4)[i + 0xa00] = data_8003dc00_slot28[i];
    }
}
