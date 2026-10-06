/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_80017560_slot12[])(void);
extern void (*data_80017568_slot12[])(void);
void func_80135c88(void);
void func_8011abe4(void);

void func_80010058_slot12(void) {
    data_80017560_slot12[data_8018f5a0->field_4c]();
    func_80135c88();
}

void func_800100a8_slot12(void) {
    data_80017568_slot12[data_8018f5a0->field_4e]();
    func_80138164();
    func_8011abe4();
}
