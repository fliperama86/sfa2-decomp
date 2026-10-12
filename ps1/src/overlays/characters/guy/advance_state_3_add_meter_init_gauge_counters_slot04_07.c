/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void advance_state_3_add_meter_init_gauge_counters_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->gauge = 0xb4;
    obj->countdown = 0;
    ((Slot04aObj *)obj)->field_1c8 = 0;
    ((Slot04aObj *)obj)->field_1c9 = 0;
    ((Slot04aObj *)obj)->field_1ce = 0;
    set_sequence_by_side_and_mode(obj, 0x19);
}
