/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80153328(Effect *effect) {
    if (effect->owner->effect_request != 0) {
        effect->active_flag = 1;
        effect->field_04 = 0xff;
        effect->state_index++;
        effect->table_index = effect->owner->effect_request >> 1;
        func_80153388(effect);
    }
}

void func_80153388(Effect *effect) {
    effect->countdown_timer = 0x20;
    effect->field_0b = 0xff;
    effect->field_04 = 0xff;
    effect->substep_index = 0;
    effect->owner->effect_request = 0;
    func_80153d7c(effect);
    if (effect->owner->mode_flag_2 == 0) {
        effect->field_10 = table_8018031c[effect->table_index];
        table_8018049c[effect->side]->field_06 = 0x50;
        func_80153dfc(effect);
    }
}

void func_80153430(Effect *effect) {
    if (effect->owner->effect_request != 0) {
        effect->table_index = effect->owner->effect_request >> 1;
        func_80153388(effect);
    } else {
        func_80153d7c(effect);
        if (effect->owner->mode_flag_2 == 0) {
            func_80153dfc(effect);
        }
        effect->countdown_timer = effect->countdown_timer - 1;
        if (effect->countdown_timer & 0x8000) {
            effect->state_index = 0;
            effect->active_flag = 0;
            effect->field_04 = 0;
        }
    }
}

void func_801534cc(Effect *effect) {
    effect->target_count = effect->target->round_count;
    if (effect->target_count >= 2) {
        func_80153514(effect);
    }
}

void func_80153514(Effect *effect) {
    effect->substep_index = 1;
    effect->active_flag = 1;
    effect->current_count = effect->target_count;
    effect->field_10 = effect->target->field_170;
    effect->table_index = 6;
    func_80153d7c(effect);
    effect->field_09 = effect->current_count;
    table_801803c4[effect->side]->text_x = table_80180334[effect->side];
    queue_two_digit_number_text(effect);
}
