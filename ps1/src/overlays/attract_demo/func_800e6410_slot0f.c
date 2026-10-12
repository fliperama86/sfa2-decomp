/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u32 data_800ebcb8_slot0f[];
extern SequenceStep *data_800ef8d0_slot0f[];
extern void (*data_800efd8c_slot0f[])(Object *);

void func_800e6410_slot0f(Object *obj, int a) {
    char *base = (char *)0x80061000;
    obj->field_90 = base;
    obj->field_90 = base + data_800ebcb8_slot0f[obj->field_60];
    set_sequence_from_table_5(obj, (a + obj->field_60 * 2) & 0xff, data_800ef8d0_slot0f);
}

void call_handler_by_state_0_2_slot0f(Object *obj) {
    data_800efd8c_slot0f[obj->state_0](obj);
}
