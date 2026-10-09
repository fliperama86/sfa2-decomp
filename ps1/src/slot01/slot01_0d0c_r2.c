/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800151b4_slot01[])(void);

void func_80010e60_slot01(void) {
    data_800151b4_slot01[data_8018f5a0->field_4e]();
    func_80138164();
    func_8011abe4();
}
