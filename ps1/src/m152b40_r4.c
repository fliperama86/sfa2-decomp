/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801535a4(Effect *effect) {
    func_80153d7c(effect);
    queue_two_digit_number_text(effect);
    effect->target_count = effect->target->round_count;
    if (effect->target_count != 0) {
        if (effect->current_count < effect->target_count) {
            func_80153514(effect);
        }
    } else {
        effect->countdown_timer = 0x20;
        effect->table_index = 0;
        effect->substep_index++;
        func_80153d7c(effect);
        effect->field_09 = effect->current_count;
        table_801803c4[effect->side]->text_x = table_80180338[effect->side];
        queue_two_digit_number_text(effect);
        if (effect->owner->mode_flag_2 == 0) {
            table_8018049c[effect->side]->field_06 = 0x50;
            func_80153dfc(effect);
        }
    }
}
