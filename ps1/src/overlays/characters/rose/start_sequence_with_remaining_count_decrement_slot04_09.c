/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_with_remaining_count_decrement_slot04_09(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    obj->field_157 = 0;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    play_side_sound_at(obj, obj->side, 0xd);
    set_sequence_by_side_and_mode(obj, 0x34);
}
