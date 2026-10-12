/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_handler_by_state_3_slot04_07(Object *obj);
void call_state_3_handler_then_play_step_sound_7_slot04_07(Object *obj);

void call_handler_by_input_dir_1_slot04_07(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second == 0) {
        call_handler_by_state_3_slot04_07(obj);
    } else {
        call_state_3_handler_then_play_step_sound_7_slot04_07(obj);
    }
}
