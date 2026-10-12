/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7800_slot04_02[];

void func_801b72f8_slot04_02(Object *o) {
    call_state_2_handler_2684_unless_paused(o);
}

void call_handler_by_state_2_3_slot04_02(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7800_slot04_02[o->state_2](o);
    }
    func_8011ffdc(o);
}
