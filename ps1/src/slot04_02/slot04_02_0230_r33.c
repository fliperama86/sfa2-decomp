/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7800_slot04_02[];

void func_801b72f8_slot04_02(Object *o) {
    func_80137cc0(o);
}

void func_801b7318_slot04_02(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7800_slot04_02[o->field_06](o);
    }
    func_8011ffdc(o);
}
