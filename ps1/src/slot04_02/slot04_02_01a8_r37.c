/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4b68_slot04_02(Object *obj) {
    obj->state_3 = 0x11;
    obj->active_flag_3 = 1;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x5a);
}
