/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c1e44_slot04_07[];
void play_sound_from_step_flags_table_slot04_07(Object *o);

void func_801b0388_slot04_07(Object *obj) {
    data_801c1e44_slot04_07[obj->state_2](obj);
    play_sound_from_step_flags_table_slot04_07(obj);
}
