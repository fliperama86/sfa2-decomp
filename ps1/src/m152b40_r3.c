/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80153230(Effect *effect) {
    Object *owner = effect->owner;
    Object *target = effect->target;
    if (owner->field_cc <= target->field_6a) {
        owner->field_cc = target->field_6a;
    }
    if (game_state.field_ac != 0) {
        func_80153c7c(effect);
    } else if ((s16)target->field_5c & 0x8000) {
        func_801538f0(effect);
    } else {
        table_80180304[effect->field_03](effect);
        if (effect->field_04 == 0) {
            table_8018030c[effect->field_05](effect);
        }
    }
}
