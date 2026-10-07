/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern int data_800f8578_slot0f;

void func_800e4800_slot0f(void) {
    data_800f8578_slot0f = 0;
    data_8018f5a0->field_52 = 0;
    func_80120554(0, 0, 0x205);
}
