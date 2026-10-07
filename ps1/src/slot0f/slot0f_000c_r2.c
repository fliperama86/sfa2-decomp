/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800e841c_slot0f[])(Object *);
extern void (*data_800e8428_slot0f[])(Object *);

void func_800e0150_slot0f(Object *obj) {
    data_800e841c_slot0f[data_8018f5a0->field_4a](obj);
}

void func_800e0198_slot0f(Object *obj) {
    data_800e8428_slot0f[data_8018f5a0->field_4c](obj);
}
