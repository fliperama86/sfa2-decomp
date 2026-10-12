/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b360c_slot04_02(Object *obj) {
    obj->field_225 = 1;
    obj->state_3 = 1;
    if (obj->field_7e == 0) {
        func_80145d20(obj);
    }
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3c);
}
