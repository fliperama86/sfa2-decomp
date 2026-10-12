/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void update_effect_count_text_countdown(Effect *effect) {
    u8 t;
    if (effect->target->round_count >= 2) {
        init_effect_count_text(effect);
        return;
    }
    queue_text_from_table(effect);
    queue_two_digit_number_text(effect);
    if (effect->owner->mode_flag_2 == 0) {
        format_score_text(effect);
    }
    effect->countdown_timer = effect->countdown_timer - 1;
    if (effect->countdown_timer & 0x8000) {
        effect->countdown_timer = 0x20;
        effect->substep_index++;
        t = effect->current_count;
        effect->field_09 = t;
        if (t == 0) {
            effect->active_flag = 0;
            effect->substep_index = 0;
            return;
        }
        effect->field_09 = t - 2;
        if (effect->field_09 >= 12) {
            effect->field_09 = 12;
        }
        effect->field_09 = effect->field_09 + 10;
        effect->table_index = effect->field_09;
        queue_text_from_table(effect);
        if (effect->owner->mode_flag_2 == 0) {
            effect->field_09 = effect->current_count;
            if (effect->field_09 >= 100) {
                effect->field_09 = 99;
            }
            if (effect->owner->mode_flag_2 == 0) {
                effect->field_10 = table_80181380[effect->field_09];
                table_8018049c[effect->side]->field_06 = 0x50;
                format_score_text(effect);
            }
        }
    }
}
