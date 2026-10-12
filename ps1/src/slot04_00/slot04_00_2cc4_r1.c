/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_41_set_flags_slot04_00(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->field_29c = 2;
    set_sequence_by_side_and_mode(obj, 0x41);
}
