/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_effect_on_flag_or_finish_slot04_02(Object *obj) {
    s16 t;

    t = obj->step_flags;
    if (t < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        spawn_offset_effect_and_play_sounds(obj);
    } else {
        update_sequence(obj);
    }
}
