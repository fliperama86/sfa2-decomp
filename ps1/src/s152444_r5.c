/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_kind_table_for_two_effects(void) {
    Effect *first = data_8018d210;
    Effect *second;

    table_801802f4[first->kind](first);
    second = first + 1;
    table_801802f4[second->kind](first + 1);
}

void init_effect_owner_target_from_side(Effect *effect) {
    Object *first = &player_left;
    Object *second = &player_left + 1;

    effect->kind++;
    effect->active_flag = 0;
    effect->state_index = 0;
    effect->field_04 = 0;
    effect->substep_index = 0;
    effect->current_count = 0;
    if (effect->side != 0) {
        Object *tmp = first;
        first = second;
        second = tmp;
    }
    effect->owner = first;
    effect->target = second;
    update_effect_by_state_tables(effect);
}
