/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80017560_slot12[])(void);
extern void (*data_80017568_slot12[])(void);

void func_80010058_slot12(void) {
    data_80017560_slot12[data_8018f5a0->step_index]();
    func_80135c88();
}

void func_800100a8_slot12(void) {
    data_80017568_slot12[data_8018f5a0->substep_index]();
    func_80138164();
    func_8011abe4();
}
