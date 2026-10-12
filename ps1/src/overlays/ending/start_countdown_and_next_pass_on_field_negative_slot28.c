/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051b18_slot28[];
extern ObjectRef data_80051b50_slot28;

void start_countdown_and_next_pass_on_field_negative_slot28(Object *obj) {
    Object *p;
    p = data_80051b50_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        data_8018f5a0->countdown_timer = 0x3c;
        p = data_80051b18_slot28[0];
        p->variant_index = 0xff;
        data_8018f5a0->pass_index++;
    }
}
