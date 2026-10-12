/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_at_other_box_add_meter_slot04_09(Object *obj, Object *unused) {
    obj->state_3++;
    if (obj->counter == 0) {
        add_to_meter_clamped(obj, 3);
    }
    play_sound_at(obj, obj->side ^ 1, 0x31b);
    set_sequence_by_side_and_mode(obj, 0x18);
    set_position_from_other_box(obj);
}
