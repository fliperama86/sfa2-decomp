/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b02bc_slot04_01(Object *object) {
    s16 t = object->field_46;

    if (t != 0) {
        t = t - 1;
        object->field_46 = t;
        if (t == 0) {
            game_state.config->field_4b |= object->side + 1;
        }
    }
    func_80130efc(object);
}
