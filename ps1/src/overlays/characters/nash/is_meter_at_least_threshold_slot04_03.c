/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int is_meter_at_least_threshold_slot04_03(Object *obj) {
    return (s16)obj->meter >= 0x30;
}
