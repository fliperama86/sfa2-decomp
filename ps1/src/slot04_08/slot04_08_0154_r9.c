/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_3_and_spawn_effect_with_sounds_slot04_08(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    spawn_offset_effect_and_play_sounds(obj);
}
