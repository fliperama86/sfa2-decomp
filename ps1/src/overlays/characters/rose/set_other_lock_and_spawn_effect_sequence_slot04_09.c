/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_other_lock_and_spawn_effect_sequence_slot04_09(Object *obj) {
    obj->other_lock = 0x14;
    obj->field_157 = 0;
    obj->state_3++;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x37);
}
