/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_8005198c_slot28;

void func_80018418_slot28(Object *obj) {
    if ((s16)data_8005198c_slot28.p->step_flags < 0) {
        data_8018f5a0->pass_index++;
    }
}
