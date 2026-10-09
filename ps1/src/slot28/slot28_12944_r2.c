/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051bdc_slot28;

void func_80022afc_slot28(Object *obj) {
    if (data_80190949 != 0 && *(s16 *)&data_80051bdc_slot28.p->field_3a < 0) {
        data_8018f5a0->field_60 = 0xb4;
        data_8018f5a0->field_52++;
    }
}
