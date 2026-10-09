/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Poly28 data_80051df4_slot28[];

void func_800281ec_slot28(Object *obj) {
    ((PrimTag *)&data_80051df4_slot28[data_801a27d0])->addr = ((PrimTag *)data_801987c8)[26].addr;
    ((PrimTag *)data_801987c8)[26].addr = (u32)&data_80051df4_slot28[data_801a27d0];
    ((PrimTag *)&data_80051df4_slot28[data_801a27d0 + 2])->addr = ((PrimTag *)data_801987c8)[26].addr;
    ((PrimTag *)data_801987c8)[26].addr = (u32)&data_80051df4_slot28[data_801a27d0 + 2];
}
