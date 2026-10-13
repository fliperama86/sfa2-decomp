/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80019054_slot12[];
extern u8 data_80019548_slot12[];
extern SequenceStep *data_8001d4ec_slot12[];
extern int data_8002d57c_slot12;

void update_sequence_with_end_flags_slot12(Object *obj) {
    u16 *in = &data_801ae066;

    *in = obj->step_flags;
    if (*in & 0x8000) {
        obj->field_01 = 0;
        obj->state_0++;
        game_state.busy_flag = 0;
        single_object_active = 0;
    }
    if (data_8002d57c_slot12 != 0) {
        advance_sequence_checked(obj);
    }
    if (*in & 0x80) {
        data_8002d57c_slot12 = 0;
    }
}
