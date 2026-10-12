/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void mark_hit_and_clear_hit_count(Object *a0, Object *a1) {
    if (a0->wide_boxes[a0->frame->active].box_type == 0x1d) {
        a1->action_timer = 0x30;
    }
    a1->hit_type = 0xff;
    data_80188f34++;
    if (a0->sound_side != 0) {
        a1->hit_count_b = 0;
    } else {
        a1->hit_count_a = 0;
    }
}

void hit_response_by_flags(Object *a0, Object *a1, Box32 *a2) {
    if (a0->field_4b != 0 ||
        (a0->mode_id != 8 && a0->active_state != 0 && (a2->hit_flags & 0x80) == 0) ||
        (a0->mode_id == 8 && (a2->hit_flags & 0x80) == 0)) {
        mark_hit_and_clear_hit_count(a0, a1);
    } else {
        dispatch_hit_handler_by_box_type(a0, a1, a2);
    }
}

void dispatch_hit_handler_by_box_type(Object *a0, Object *a1, Box32 *a2) {
    if (a0->field_7e != 0 && a0->field_7e < 10) {
        a1->hit_type = 7;
        a1->field_15b = 1;
    } else {
        if (a1->hit_type == 0x1d) {
            a1->hit_type = 0x12;
        }
        if (a1->active_state != 0) {
            handlers_70a4[a2->box_type](a0, a1, a2);
        } else {
            handlers_702c[a2->box_type](a0, a1, a2);
        }
    }
}
