/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_call_effect_on_low_flag_slot04_00(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x3a);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    }
}
