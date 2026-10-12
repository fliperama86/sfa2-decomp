/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_effect_and_set_other_code_six_on_flags_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3++;
        spawn_offset_effect_and_play_sounds(obj);
        obj->other->field_6b = 6;
    }
    update_sequence(obj);
}

void start_next_sequence_when_step_flags_negative_slot04_03(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x20);
    }
    update_sequence(obj);
}
