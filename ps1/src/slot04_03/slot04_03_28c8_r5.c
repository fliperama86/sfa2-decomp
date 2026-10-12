/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_spawn_effect_on_step_flags_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3++;
        spawn_offset_effect_and_play_sounds(obj);
        obj->other->field_6b = 0x12;
    }
    update_sequence(obj);
}

void start_alt_sequence_when_step_flags_negative_slot04_03(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x42);
    }
    update_sequence(obj);
}
