/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void hit_response_by_box_type_4(Object *a0, Object *a1, Box32 *a2) {
    if (a1->field_d8 != 0) {
        mark_hit_and_clear_hit_count(a0, a1);
    } else if (a2->box_type != 4) {
        set_hit_type_from_box_type_with_counter(a0, a1, a2);
    } else {
        a1->field_15b = 1;
        a1->hit_type = a2->box_type;
        if (a1->hit_type == 0x1d) {
            a1->hit_type = 0x12;
        }
    }
}
