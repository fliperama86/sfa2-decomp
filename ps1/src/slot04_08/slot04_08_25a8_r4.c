/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2a8c_slot04_08(Object *obj) {

    obj->substate_index++;
    func_80145d20(obj);
    ((Slot04aObj *)obj)->sub_index = 0;
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x40);
}
