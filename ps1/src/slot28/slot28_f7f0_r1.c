/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051ad8_slot28[];
void func_80128410(void);

void func_8001f7f0_slot28(Object *obj) {
    if (data_80051ad8_slot28[0]->field_07 == 1) {
        data_8018f5a0->field_52++;
        func_80128410();
    }
}
