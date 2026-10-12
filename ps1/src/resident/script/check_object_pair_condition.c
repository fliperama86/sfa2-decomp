/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

u8 check_object_pair_condition(Object *a, Object *b) {
    if (a->active_state != 0 && b->active_state == 0) return 0;
    if (b->field_225 != 0) return 1;
    if (b->frame->field_0c & 0x80) return 0;
    if (b->active_flag_3 == 0) return 0;
    return (u8)is_other_active_frame_in_x_range(a, b) != 0;
}

u8 is_other_in_x_range_and_field_zero(Object *a, Object *b, s16 *c, int d) {
    if ((u16)((u16)b->pos_x - (u16)a->pos_x + 0x80) <= 0x100) {
        return is_input_matched_flag_clear(b) != 0;
    } else {
        return 0;
    }
}
