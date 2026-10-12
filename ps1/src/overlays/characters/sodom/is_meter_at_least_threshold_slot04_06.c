/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int is_meter_at_least_threshold_slot04_06(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int return_true_always_slot04_06(Object *obj) {
    return 1;
}

int is_meter_at_least_threshold_2_slot04_06(Object *obj) {
    return (s16)obj->meter >= 0x30;
}
