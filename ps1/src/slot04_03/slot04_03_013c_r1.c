/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b013c_slot04_03(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void func_801b0170_slot04_03(Object *obj) {
    if (game_state.field_64 == 0 && game_state.countdown_timer == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}
