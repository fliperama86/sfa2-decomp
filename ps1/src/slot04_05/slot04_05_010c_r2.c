/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b01a0_slot04_05(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->field_0b = obj->field_158;
    set_sequence_by_side(obj, 0);
}

void func_801b01d4_slot04_05(Object *obj) {
    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}
