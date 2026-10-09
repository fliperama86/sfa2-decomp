/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_80017574_slot12[])(void);
void func_8001129c_slot12(void);

void func_80010228_slot12(void) {
    data_80017574_slot12[data_8018f5a0->field_4e]();
    func_80138164();
    func_8011abe4();
    func_8001129c_slot12();
}
