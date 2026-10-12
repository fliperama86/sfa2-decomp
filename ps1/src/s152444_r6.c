/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80153328(Effect *effect) {
    if (effect->owner->field_bd != 0) {
        effect->active_flag = 1;
        effect->field_04 = 0xff;
        effect->field_03++;
        effect->table_index = effect->owner->field_bd >> 1;
        func_80153388(effect);
    }
}

void func_80153388(Effect *effect) {
    effect->countdown_timer = 0x20;
    effect->field_0b = 0xff;
    effect->field_04 = 0xff;
    effect->field_05 = 0;
    effect->owner->field_bd = 0;
    func_80153d7c(effect);
    if (effect->owner->field_cd == 0) {
        effect->field_10 = table_8018031c[effect->table_index];
        table_8018049c[effect->side]->field_06 = 0x50;
        func_80153dfc(effect);
    }
}

void func_80153430(Effect *effect) {
    if (effect->owner->field_bd != 0) {
        effect->table_index = effect->owner->field_bd >> 1;
        func_80153388(effect);
    } else {
        func_80153d7c(effect);
        if (effect->owner->field_cd == 0) {
            func_80153dfc(effect);
        }
        effect->countdown_timer = effect->countdown_timer - 1;
        if (effect->countdown_timer & 0x8000) {
            effect->field_03 = 0;
            effect->active_flag = 0;
            effect->field_04 = 0;
        }
    }
}

void func_801534cc(Effect *effect) {
    effect->field_0a = effect->target->field_6a;
    if (effect->field_0a >= 2) {
        func_80153514(effect);
    }
}

void func_80153514(Effect *effect) {
    effect->field_05 = 1;
    effect->active_flag = 1;
    effect->field_06 = effect->field_0a;
    effect->field_10 = effect->target->field_170;
    effect->table_index = 6;
    func_80153d7c(effect);
    effect->field_09 = effect->field_06;
    table_801803c4[effect->side]->field_04 = table_80180334[effect->side];
    queue_two_digit_number_text(effect);
}
