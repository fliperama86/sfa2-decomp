/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_state3_first_frame_range_check_narrow_slot04_04(Object *obj);
void update_state_3_start_move_with_sound_slot04_04(Object *obj);

void call_handler_by_input_dir_second_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        update_state_3_start_move_with_sound_slot04_04(obj);
    } else {
        update_state3_first_frame_range_check_narrow_slot04_04(obj);
    }
}
