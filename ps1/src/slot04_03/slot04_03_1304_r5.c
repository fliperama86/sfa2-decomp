/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b17d0_slot04_03(Object *obj);
void call_handler_by_direction_index_slot04_03(Object *obj);

void call_handler_by_substate_index_slot04_03(Object *obj) {
    if (obj->substate_index == 0) {
        func_801b17d0_slot04_03(obj);
    } else {
        call_handler_by_direction_index_slot04_03(obj);
    }
}

void func_801b17d0_slot04_03(Object *obj) {
    obj->substate_index++;
    func_80145d20(obj);
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3b);
}
