/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051c1c_slot28;
void func_801280f0(void);

void func_80023a80_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051c1c_slot28.p)->field_3a < 0) {
        func_801280f0();
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
    }
}
