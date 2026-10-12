/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_effect_round_count_text(Effect *effect) {
    queue_text_from_table(effect);
    queue_two_digit_number_text(effect);
    effect->target_count = effect->target->round_count;
    if (effect->target_count != 0) {
        if (effect->current_count < effect->target_count) {
            init_effect_count_text(effect);
        }
    } else {
        effect->countdown_timer = 0x20;
        effect->table_index = 0;
        effect->substep_index++;
        queue_text_from_table(effect);
        effect->field_09 = effect->current_count;
        table_801803c4[effect->side]->text_x = table_80180338[effect->side];
        queue_two_digit_number_text(effect);
        if (effect->owner->mode_flag_2 == 0) {
            table_8018049c[effect->side]->field_06 = 0x50;
            format_score_text(effect);
        }
    }
}
