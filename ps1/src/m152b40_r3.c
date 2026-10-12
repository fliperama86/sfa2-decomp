/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void update_effect_by_state_tables(Effect *effect) {
    Object *owner = effect->owner;
    Object *target = effect->target;
    if (owner->max_value_cc <= target->round_count) {
        owner->max_value_cc = target->round_count;
    }
    if (game_state.mode_flag != 0) {
        clear_field_and_increment_step_count(effect);
    } else if ((s16)target->health & 0x8000) {
        show_round_text_by_owner_effect_code(effect);
    } else {
        table_80180304[effect->state_index](effect);
        if (effect->field_04 == 0) {
            table_8018030c[effect->substep_index](effect);
        }
    }
}
