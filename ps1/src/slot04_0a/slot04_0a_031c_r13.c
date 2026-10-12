/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b321c_slot04_0a(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    spawn_offset_effect_and_play_sounds(obj);
}
