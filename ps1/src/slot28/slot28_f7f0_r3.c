/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051b00_slot28[];
void func_8002005c_slot28(Object *obj, int arg);
void func_801282d4(void);

void func_8001fc0c_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        func_8014f4d4(6, 1);
        data_8018f5a0->field_52++;
        func_8002005c_slot28(obj, 2);
    }
}

void func_8001fc68_slot28(Object *obj) {
    if ((s16)data_80051b00_slot28[4]->field_3a < 0) {
        data_8018f5a0->field_52++;
        func_8014f4d4(1, 0x400);
        func_801282d4();
    }
}
