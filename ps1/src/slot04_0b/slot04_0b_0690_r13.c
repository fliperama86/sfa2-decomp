/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2efc_slot04_0b[];

void call_handler_by_state_2_unless_paused_slot04_0b(Object *obj) {
    if ((game_state.config->pause_flag_b | game_state.config->pause_flag_a) != 0) {
        func_8011ffdc(obj);
    } else {
        data_801c2efc_slot04_0b[obj->state_2](obj);
        func_8011ffdc(obj);
    }
}
