/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6354_slot04_02[];

void update_sequence_or_call_finish_on_flag_2_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b297c_slot04_02(Object *obj) {
    data_801c6354_slot04_02[obj->state_3](obj);
}
