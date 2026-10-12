/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enter_state_set_gauge_start_sequence_1a_sound_slot04_08(Object *obj, Object *unused) {
    Slot04aObj *o = (Slot04aObj *)obj;

    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->gauge = 0x60;
    obj->countdown = 0;
    o->done_flag = 0;
    set_sequence_by_side_and_mode(obj, 0x1a);
}
