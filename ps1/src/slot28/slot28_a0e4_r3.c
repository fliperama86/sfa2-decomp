/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_800519d0_slot28;

void func_8001a644_slot28(Object *obj) {
    if ((s16)data_800519d0_slot28.p->step_flags < 0) {
        func_8014f4d4(1, 0x502);
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        func_801282d4();
    }
}
