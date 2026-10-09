/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051890_slot28;
extern ObjectRef data_800518c8_slot28;
void func_8001581c_slot28(Object *obj, int arg);

void func_800156b0_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        p = data_800518c8_slot28.p;
        if (((Slot28Obj *)p)->field_3a < 0) {
            HudState *h = data_8018f5a0;
            h->field_52 = h->field_52 + 1;
            p = data_80051890_slot28.p;
            p->field_48 = 0xff;
            func_8001581c_slot28(obj, 3);
        }
    }
}
