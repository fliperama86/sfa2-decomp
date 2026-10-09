/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051a84_slot28[];
void func_80128370(void);
void func_8001e2ec_slot28(Object *obj, int arg);
void func_8001e37c_slot28(void);

void func_8001dfd4_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 += 1;
        func_8001e37c_slot28();
        data_80051a84_slot28[0]->pos_y = -0x40;
        func_80128370();
        func_8001e2ec_slot28(obj, 1);
    }
}
