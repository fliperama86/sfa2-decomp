/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1aa4_slot04_0a(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->active_flag = 0;
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x22);
    }
}
