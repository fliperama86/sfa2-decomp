/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bf0d8_slot04_01[];

void update_sequence_or_call_finish_on_flag_slot04_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b2c54_slot04_01(Object *obj) {
    data_801bf0d8_slot04_01[obj->state_3](obj);
}
