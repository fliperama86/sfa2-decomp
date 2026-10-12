/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80153230(Effect *effect) {
    Object *owner = effect->owner;
    Object *target = effect->target;
    if (owner->max_value_cc <= target->round_count) {
        owner->max_value_cc = target->round_count;
    }
    if (game_state.mode_flag != 0) {
        func_80153c7c(effect);
    } else if ((s16)target->health & 0x8000) {
        func_801538f0(effect);
    } else {
        table_80180304[effect->state_index](effect);
        if (effect->field_04 == 0) {
            table_8018030c[effect->substep_index](effect);
        }
    }
}
