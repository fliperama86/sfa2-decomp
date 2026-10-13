/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFnInt data_801c0578_slot04_0a[];

void func_801b0c88_slot04_0a(Object *obj) {
    script_condition_result = data_801c0578_slot04_0a[obj->field_15a](obj);
}

int return_true_always_slot04_0a(Object *obj) {
    return 1;
}

int is_link_flags_zero_slot04_0a(Object *obj) {
    return obj->link_flags == 0;
}
