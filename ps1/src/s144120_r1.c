/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void move_until_countdown_set_pos_play_sound(Object *object) {
    if (*(u8 *)&object->step_flags == 0) {
        update_position_x_with_accel(object);
        object->countdown = (s16)object->countdown - 1;
        if (object->countdown & 0x80) {
            object->pos_x = 0xc0;
            object->state_2 = object->state_2 + 1;
            play_sound_at(0, 0, 0x20a);
        }
    }
}

void call_next_state_0_on_sequence_end(Object *object) {
    if ((s16)object->step_flags < 0) {
        advance_state_0_and_clear_config_mode(object);
    }
}
