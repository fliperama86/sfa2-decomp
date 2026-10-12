/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c542c_slot04_06[];

void update_sequence_or_finish_sequence_2_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b3044_slot04_06(Object *obj) {
    data_801c542c_slot04_06[obj->state_3](obj);
}
