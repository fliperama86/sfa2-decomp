/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void hit_response_by_versus_flag(Object *a0, Object *a1, Box32 *unused) {
    if (game_state.versus_flag) {
        if (a1->hit_count_2) {
call:
            mark_hit_and_clear_hit_count(a0, a1);
        }
    } else {
        if (a1->field_d8 != 0) goto call;
        dispatch_hit_handler_by_box_type(a0, a1, &a0->wide_boxes[a0->frame->active]);
    }
}
