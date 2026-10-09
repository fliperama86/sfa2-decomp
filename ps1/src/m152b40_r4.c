/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801535a4(Effect *effect) {
    func_80153d7c(effect);
    func_80153cac(effect);
    effect->field_0a = effect->target->field_6a;
    if (effect->field_0a != 0) {
        if (effect->field_06 < effect->field_0a) {
            func_80153514(effect);
        }
    } else {
        effect->field_0c = 0x20;
        effect->field_08 = 0;
        effect->field_05++;
        func_80153d7c(effect);
        effect->field_09 = effect->field_06;
        table_801803c4[effect->side]->field_04 = table_80180338[effect->side];
        func_80153cac(effect);
        if (effect->owner->field_cd == 0) {
            table_8018049c[effect->side]->field_06 = 0x50;
            func_80153dfc(effect);
        }
    }
}
