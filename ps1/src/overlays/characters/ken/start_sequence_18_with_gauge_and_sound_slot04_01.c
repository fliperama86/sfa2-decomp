/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_18_with_gauge_and_sound_slot04_01(Object *obj) {
    obj->gauge = 0xb4;
    obj->countdown = 0;
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, ((Slot04aObj *)obj)->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x18);
}
