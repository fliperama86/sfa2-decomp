/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_or_reset_state_on_end_slot04_02(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            game_state.state_param = 0x18;
            play_side_sound_at(obj, obj->side, 0xe);
        }
        update_sequence(obj);
    }
}
