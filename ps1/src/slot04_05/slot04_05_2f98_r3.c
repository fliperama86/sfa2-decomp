/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_call_effect_on_step_flag_slot04_05(Object *obj) {
    u16 t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        obj->state_3++;
        spawn_offset_effect_and_play_sounds(obj);
        obj->other->field_6b = 0x14;
    }
    update_sequence(obj);
}
