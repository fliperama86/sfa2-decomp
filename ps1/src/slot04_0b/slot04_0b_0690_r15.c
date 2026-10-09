/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2f04_slot04_0b[];

void func_801b3bfc_slot04_0b(Object *obj) {
    if ((game_state.config->field_a8 | game_state.config->field_65) != 0) {
        func_8011ffdc(obj);
    } else {
        data_801c2f04_slot04_0b[obj->field_06](obj);
        func_8011ffdc(obj);
    }
}
