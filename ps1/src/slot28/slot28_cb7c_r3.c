/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051a44_slot28[];
void func_8001d548_slot28(Object *obj, int arg);
void func_8001d5d8_slot28(void);

void func_8001cf40_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_52 += 1;
        func_8001d5d8_slot28();
        func_8001d548_slot28(obj, 2);
        data_80051a44_slot28[0]->field_01 = 0;
        func_80128370();
    }
}
