/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b29bc_slot04_02(Object *obj) {
    obj->state_3++;
    obj->countdown = *(u8 *)&obj->countdown | 0x3200;
    func_80145d20(obj);
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x28);
}
