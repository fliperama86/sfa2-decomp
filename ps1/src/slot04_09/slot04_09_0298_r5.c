/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int clear_substate_counters_return_true_slot04_09(Object *obj);

int check_meter_full_and_clear_substate_slot04_09(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    return clear_substate_counters_return_true_slot04_09(obj);
}

int check_meter_and_no_link_clear_substate_slot04_09(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->link_flags != 0) return 0;
    return clear_substate_counters_return_true_slot04_09(obj);
}
