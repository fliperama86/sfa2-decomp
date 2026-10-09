/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801536b8(Effect *effect) {
    u8 t;
    if (effect->target->field_6a >= 2) {
        func_80153514(effect);
        return;
    }
    func_80153d7c(effect);
    func_80153cac(effect);
    if (effect->owner->field_cd == 0) {
        func_80153dfc(effect);
    }
    effect->field_0c = effect->field_0c - 1;
    if (effect->field_0c & 0x8000) {
        effect->field_0c = 0x20;
        effect->field_05++;
        t = effect->field_06;
        effect->field_09 = t;
        if (t == 0) {
            effect->field_02 = 0;
            effect->field_05 = 0;
            return;
        }
        effect->field_09 = t - 2;
        if (effect->field_09 >= 12) {
            effect->field_09 = 12;
        }
        effect->field_09 = effect->field_09 + 10;
        effect->field_08 = effect->field_09;
        func_80153d7c(effect);
        if (effect->owner->field_cd == 0) {
            effect->field_09 = effect->field_06;
            if (effect->field_09 >= 100) {
                effect->field_09 = 99;
            }
            if (effect->owner->field_cd == 0) {
                effect->field_10 = table_80181380[effect->field_09];
                table_8018049c[effect->side]->field_06 = 0x50;
                func_80153dfc(effect);
            }
        }
    }
}
