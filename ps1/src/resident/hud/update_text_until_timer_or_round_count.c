/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_text_until_timer_or_round_count(Effect *effect) {
    int t;
    if (((Actor *)effect)->target->round_count >= 2) {
        init_effect_count_text((Actor *)effect);
    } else {
        queue_text_from_table((Actor *)effect);
        if (((Actor *)effect)->owner->mode_flag_2 == 0) {
            format_score_text((Actor *)effect);
        }
        t = ((Actor *)effect)->countdown_timer - 1;
        ((Actor *)effect)->countdown_timer = t;
        if (t & 0x8000) {
            ((Actor *)effect)->field_02 = 0;
            ((Actor *)effect)->field_05 = 0;
        }
    }
}

void show_round_text_by_owner_effect_code(Actor *a) {
    a->field_02 = 1;
    if (a->owner->effect_code_bc != 0) {
        show_round_count_text(a);
    } else {
        show_round_count_text_or_clear(a);
    }
}
