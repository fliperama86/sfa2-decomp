/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c266c_slot04_0b[];

void update_sequence_or_call_finish_on_flag_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b31b0_slot04_0b(Object *obj) {
    data_801c266c_slot04_0b[obj->state_3](obj);
}
