/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int is_unlinked_with_meter_at_least_threshold_slot04_03(Object *obj) {
    if (obj->link_flags == 0) {
        if ((s16)obj->meter >= 0x30) {
            return 1;
        }
    }
    return 0;
}

