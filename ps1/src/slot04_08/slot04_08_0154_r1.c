/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0154_slot04_08(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0188_slot04_08(Object *obj) {
    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}
