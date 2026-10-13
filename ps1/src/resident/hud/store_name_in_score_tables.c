/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void store_name_in_score_tables(void *a, Object *object) {
    FrameRecord *r;
    u8 i;
    object->countdown_timer = 0x3c;
    object->name_char_3 = 0x20;
    object->phase_index++;
    replace_blocked_name(a, object);
    copy_top_rank_data_by_side(a, object);
    r = score_rank_table;
    i = object->score_rank - 1;
    if (i < 5) {
        r = &r[i];
        r->value_a = object->name_char_0;
        r->value_b = object->name_char_1;
        r->handler_index = object->name_char_2;
        r->box_index = object->name_char_3;
    }
    r = streak_rank_table;
    i = object->field_121 - 1;
    if (i < 5) {
        r = &r[i];
        r->value_a = object->name_char_0;
        r->value_b = object->name_char_1;
        r->handler_index = object->name_char_2;
        r->box_index = object->name_char_3;
    }
}
