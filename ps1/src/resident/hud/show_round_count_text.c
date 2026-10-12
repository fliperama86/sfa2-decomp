/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void show_round_count_text(Actor *a) {
    if (a->owner->effect_code_bc == 8) {
        show_round_text_and_score(a);
        return;
    }
    if (a->target->round_count < 2) {
        a->text_index = 8;
        queue_text_from_table(a);
    } else {
        a->text_index = 9;
        queue_text_from_table(a);
        a->number_value = a->target->round_count;
        table_801803c4[a->side]->text_x = table_80180340[a->side];
        queue_two_digit_number_text(a);
    }
    update_owner_field_text(a);
    if (a->owner->mode_flag_2 == 0) {
        set_score_value_from_target_field(a);
        if (a->owner->mode_flag_2 == 0) {
            table_8018049c[a->side]->field_06 = 0x60;
            format_score_text(a);
        }
    }
}
