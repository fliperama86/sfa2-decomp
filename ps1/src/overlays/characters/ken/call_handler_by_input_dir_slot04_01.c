/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_state_3_table_then_play_step_flags_sound_5_slot04_01(Object *obj);
void call_table_function_by_state_3_10_slot04_01(Object *obj);
void call_state_3_table_then_play_step_flags_sound_4_slot04_01(Object *obj);

void call_handler_by_input_dir_slot04_01(Object *obj) {
    if (obj->input_dir_0 == 4) {
        call_state_3_table_then_play_step_flags_sound_5_slot04_01(obj);
    } else if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_10_slot04_01(obj);
    } else {
        call_state_3_table_then_play_step_flags_sound_4_slot04_01(obj);
    }
}
