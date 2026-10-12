/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void copy_side_palette_from_table_and_upload_slot27(Object *obj);

void start_slide_out_and_sync_active_state_slot27(Object *obj) {
    Object *p = obj->other;
    int v;
    int k;

    if (data_8018f5a0->substep_index >= 3 && game_state.mode != 3) {
        obj->state_0 = obj->state_0 + 1;
        v = 8;
        if (p->side == 0) {
            v = -8;
        }
        obj->vel_x = v;
    }
    k = p->field_d4;
    if (k != obj->active_state) {
        obj->active_state = k;
        copy_side_palette_from_table_and_upload_slot27(obj);
    }
    advance_sequence_checked(obj);
}
