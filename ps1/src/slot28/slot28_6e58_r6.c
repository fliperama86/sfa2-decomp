/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_80032a1c_slot28[])(Object *);
extern int data_80051950_slot28;
void func_8011abe4(void);

void func_80017490_slot28(Object *obj) {
    data_80032a1c_slot28[data_8018f5a0->field_52](obj);
    func_80138164();
    if (data_80051950_slot28 == 0) {
        func_8011abe4();
    }
}
