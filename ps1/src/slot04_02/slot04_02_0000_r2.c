/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void update_sequence_with_flag_sound_or_reset_states_slot04_02(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 0, 0);
        set_sequence_by_side(obj, 0);
    } else {
        if ((t & 0xff) != 0) {
            obj->step_flags = t & 0xff;
            game_state.state_param = 0x18;
            play_side_sound_at(obj, obj->side, 0xe);
        }
        update_sequence(obj);
    }
}
