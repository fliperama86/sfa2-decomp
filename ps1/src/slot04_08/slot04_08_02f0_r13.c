/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3d5c_slot04_08[];

void update_sequence_or_call_finish_on_flag_4_slot04_08(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b35e8_slot04_08(Object *obj) {
    data_801c3d5c_slot04_08[obj->state_3](obj);
}
