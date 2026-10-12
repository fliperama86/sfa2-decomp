/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int has_meter_and_no_link_flags_slot04_0b(Object *obj) {
    int r = 0;
    if ((s16)obj->meter >= 0x30) { if (obj->link_flags == 0) return 1; }
    return r;
}
