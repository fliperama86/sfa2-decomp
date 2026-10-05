/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80153154(void) {
    Effect *first = data_8018d210;
    Effect *second;

    table_801802f4[first->kind](first);
    second = first + 1;
    table_801802f4[second->kind](first + 1);
}

void func_801531cc(Effect *effect) {
    Object *first = &player_left;
    Object *second = &player_left + 1;

    effect->kind++;
    effect->field_02 = 0;
    effect->field_03 = 0;
    effect->field_04 = 0;
    effect->field_05 = 0;
    effect->field_06 = 0;
    if (effect->side != 0) {
        Object *tmp = first;
        first = second;
        second = tmp;
    }
    effect->owner = first;
    effect->target = second;
    func_80153230(effect);
}
