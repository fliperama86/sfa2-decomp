/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2b68_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3++;
        spawn_offset_effect_and_play_sounds(obj);
        obj->other->field_6b = 6;
    }
    update_sequence(obj);
}

void func_801b2bc4_slot04_03(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x20);
    }
    update_sequence(obj);
}
