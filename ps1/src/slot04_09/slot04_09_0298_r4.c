/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c7be0_slot04_09[];
int clear_substate_counters_return_true_slot04_09(Object *obj);

void func_801b16f0_slot04_09(Object *obj) {
    data_801ad398 = data_801c7be0_slot04_09[obj->field_15a](obj);
}

int check_no_link_flags_and_clear_substate_slot04_09(Object *obj) {
    if (obj->link_flags != 0) return 0;
    return clear_substate_counters_return_true_slot04_09(obj);
}
