/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_with_countdown_side_bit_slot04_02(Object *obj) {
    u16 t = obj->step_flags;
    s16 c;
    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        game_state.state_param = 0x18;
        play_side_sound_at(obj, obj->side, 0xe);
    }
    c = obj->countdown;
    if (c != 0) {
        c = c - 1;
        obj->countdown = c;
        if (c == 0) {
            game_state.side_bits_2 |= 1 << obj->side;
        }
    }
    update_sequence(obj);
}
