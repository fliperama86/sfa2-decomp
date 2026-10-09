/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_800519d0_slot28;

void func_8001a644_slot28(Object *obj) {
    if ((s16)data_800519d0_slot28.p->field_3a < 0) {
        func_8014f4d4(1, 0x502);
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_801282d4();
    }
}
