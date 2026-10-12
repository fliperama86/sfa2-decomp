/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2df8_slot04_09(Object *obj) {
    obj->active_flag = 0;
    obj->step_count = 0;
    obj->field_a3 = 0;
    obj->substate_index++;
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->counter == 0) {
        func_80145d20(obj);
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x30);
}
