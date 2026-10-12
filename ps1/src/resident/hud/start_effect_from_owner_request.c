/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void start_effect_from_owner_request(Effect *effect) {
    if (effect->owner->effect_request != 0) {
        effect->active_flag = 1;
        effect->field_04 = 0xff;
        effect->state_index++;
        effect->table_index = effect->owner->effect_request >> 1;
        start_effect_text_countdown(effect);
    }
}

void start_effect_text_countdown(Effect *effect) {
    effect->countdown_timer = 0x20;
    effect->field_0b = 0xff;
    effect->field_04 = 0xff;
    effect->substep_index = 0;
    effect->owner->effect_request = 0;
    queue_text_from_table(effect);
    if (effect->owner->mode_flag_2 == 0) {
        effect->field_10 = table_8018031c[effect->table_index];
        table_8018049c[effect->side]->field_06 = 0x50;
        format_score_text(effect);
    }
}

void update_effect_text_countdown(Effect *effect) {
    if (effect->owner->effect_request != 0) {
        effect->table_index = effect->owner->effect_request >> 1;
        start_effect_text_countdown(effect);
    } else {
        queue_text_from_table(effect);
        if (effect->owner->mode_flag_2 == 0) {
            format_score_text(effect);
        }
        effect->countdown_timer = effect->countdown_timer - 1;
        if (effect->countdown_timer & 0x8000) {
            effect->state_index = 0;
            effect->active_flag = 0;
            effect->field_04 = 0;
        }
    }
}

void start_effect_if_round_count_ge_2(Effect *effect) {
    effect->target_count = effect->target->round_count;
    if (effect->target_count >= 2) {
        init_effect_count_text(effect);
    }
}

void init_effect_count_text(Effect *effect) {
    effect->substep_index = 1;
    effect->active_flag = 1;
    effect->current_count = effect->target_count;
    effect->field_10 = effect->target->field_170;
    effect->table_index = 6;
    queue_text_from_table(effect);
    effect->field_09 = effect->current_count;
    table_801803c4[effect->side]->text_x = table_80180334[effect->side];
    queue_two_digit_number_text(effect);
}
