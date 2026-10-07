/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_8001514c_slot01[])(void);

void func_800100a4_slot01(void) {
    data_8001514c_slot01[data_8018f5a0->field_4e]();
    func_80138164();
    func_801510bc();
    func_8011a784();
}
