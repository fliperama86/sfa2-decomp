/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void play_sounds_on_step_flag_1_2_slot04_0b(Object *obj) {
    if (*(u8 *)&obj->step_flags == 1) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xf);
        play_side_sound_at(obj, obj->side, 4);
    }
    update_sequence(obj);
}
