/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void set_hit_type_3_with_box_check(Object *a0, Object *a1, Box32 *a2) {
    a1->hit_type = 3;
    if (a2->box_type != 0x10) {
        if ((a2->box_flags & 0x80) || a2->box_type == 7 || a2->box_type == 4 || a2->box_type == 0x1c) {
            a1->field_15b = 1;
        }
    }
}

void set_hit_type_or_default_by_in_compare(Object *a0, Object *a1, Box32 *a2) {
    if (data_80188ed0.in_04 > data_80188ed0.in_00) {
        set_hit_type_from_box_type_with_counter(a0, a1, a2);
    } else {
        a1->hit_type = 1;
    }
}
