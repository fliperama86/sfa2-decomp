/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void play_side_sound_from_step_flags_slot04_01(Object *obj);
extern ObjectFn data_801bf10c_slot04_01[];

void func_801b35ec_slot04_01(Object *obj) {
    data_801bf10c_slot04_01[obj->state_3](obj);
    play_side_sound_from_step_flags_slot04_01(obj);
}
