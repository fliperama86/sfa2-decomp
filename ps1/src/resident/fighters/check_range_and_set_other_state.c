/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


int check_range_and_set_other_state(Object *object, s16 a, int b, int c, int d) {
    if (object->facing_side != 0) a = -a;
    a = a + *(u16 *)&object->pos_x - *(u16 *)&ref_other.p->pos_x + b;
    if ((u16)a > (u16)(b * 2)) return 0;
    if ((u16)(c - *(u16 *)&object->pos_y + *(u16 *)&ref_other.p->pos_y + d) > (u16)(d * 2)) return 0;
    ref_other.p->state_0 = 1;
    ref_other.p->state_1 = 3;
    ref_other.p->state_2 = 0;
    ref_other.p->state_3 = 0;
    ref_other.p->status_flag = 0xff;
    object->status_flag = 1;
    ref_other.p->field_15b = 1;
    object->active_flag_3 = 0;
    ref_other.p->active_flag_3 = 0;
    object->field_225 = 0;
    ref_other.p->field_225 = 0;
    ref_other.p->countdown = 8;
    ref_other.p->field_180 = 0;
    ref_other.p->frame_count = 0;
    ref_other.p->cursor_timer_a = 0;
    ref_other.p->cursor_timer_b = 0;
    object->field_28c = 0;
    object->damage_countdown = 0;
    object->reference_count = 0;
    return 1;
}
