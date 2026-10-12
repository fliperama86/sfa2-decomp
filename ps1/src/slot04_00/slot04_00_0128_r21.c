/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void play_side_sound_from_step_flags_slot04_00(Object *obj);
extern ObjectFn data_801bfed0_slot04_00[];

void func_801b2d44_slot04_00(Object *obj) {
    data_801bfed0_slot04_00[obj->state_3](obj);
    play_side_sound_from_step_flags_slot04_00(obj);
}
