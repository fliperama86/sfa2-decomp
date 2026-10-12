/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFnInt data_801bfd78_slot04_00[];

void func_801b11c0_slot04_00(Object *obj) {
    data_801ad398 = data_801bfd78_slot04_00[obj->field_15a](obj);
}

int return_true_always_slot04_00(Object *obj) {
    return 1;
}

int is_link_flags_zero_slot04_00(Object *obj) {
    return obj->link_flags == 0;
}

int is_active_state_set_slot04_00(Object *obj) {
    return obj->active_state != 0;
}
