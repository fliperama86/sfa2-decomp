/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_flag_157_then_call_by_input_dir_second_slot04_04(Object *obj);
void call_handler_by_input_dir_second_slot04_04(Object *obj);
void update_jump_flag_start_or_land_on_ground_slot04_04(Object *obj);

void dispatch_by_input_dir_0_slot04_04(Object *obj) {
    if (obj->input_dir_0 == 4) {
        update_jump_flag_start_or_land_on_ground_slot04_04(obj);
    } else if (obj->input_dir_0 != 0) {
        set_flag_157_then_call_by_input_dir_second_slot04_04(obj);
    } else {
        call_handler_by_input_dir_second_slot04_04(obj);
    }
}
