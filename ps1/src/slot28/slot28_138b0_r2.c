/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051c1c_slot28;

void func_80023a80_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051c1c_slot28.p)->field_3a < 0) {
        start_fade_mode_1_and_wait();
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
    }
}
