/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013a350(Object *a0, Object *a1, Box32 *a2) {
    if (a0->mode_id != 8 && a0->counter != 0 && a2->box_type == 0xd) {
        a1->hit_type = 0x17;
    } else {
        a1->hit_type = a2->box_type;
    }
    if (a1->hit_type == 0x1d) {
        a1->hit_type = 0x12;
    }
}
