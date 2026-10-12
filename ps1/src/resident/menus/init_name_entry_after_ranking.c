/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void init_name_entry_after_ranking(void *a, Object *o) {
    u8 x, y;
    o->name_char_0 = 0x41;
    o->name_char_1 = 0x60;
    o->name_char_2 = 0x60;
    o->name_char_3 = 0x20;
    o->countdown_timer_b = 0x960;
    *(s16 *)&o->field_b2 = -0x1000;
    o->name_char_count = 0;
    o->countdown_timer = 0x3c;
    o->phase_index++;
    insert_score_in_table(a, o);
    insert_score_into_ranking_table(a, o);
    x = o->score_rank;
    y = o->field_121;
    if ((u8)(x + y) >= 12) {
        finish_name_entry_update_rank_indices(a, o);
    } else {
        if (y < x) {
            x = y;
        }
        o->min_rank = x;
    }
}
