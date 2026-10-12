/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int clear_substate_counters_return_true_slot04_09(Object *obj);

int func_801b1800_slot04_09(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->counter != 0) return 0;
    return clear_substate_counters_return_true_slot04_09(obj);
}

int func_801b1844_slot04_09(Object *obj) {
    if (obj->field_7e == 0) {
        if (obj->remaining_count == 0) {
            return 0;
        }
    }
    return clear_substate_counters_return_true_slot04_09(obj);
}
