/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_effect_on_step_flag_byte_2_slot04_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        spawn_offset_effect_and_play_sounds(obj);
    }
    update_sequence(obj);
}
