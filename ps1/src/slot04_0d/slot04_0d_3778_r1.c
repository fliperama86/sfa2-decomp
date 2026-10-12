/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_by_side(Object *object, int arg);

void func_801b3778_slot04_0d(Object *object) {
    object->state_2 = object->state_2 + 1;
    object->field_0b = object->field_158;
    set_sequence_by_side(object, 0);
}

void func_801b37ac_slot04_0d(Object *object) {
    if (game_state.field_5c == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}
