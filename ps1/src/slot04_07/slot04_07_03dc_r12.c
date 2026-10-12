/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void play_sound_from_step_flags_table_slot04_07(Object *obj);
extern ObjectFn data_801c1f64_slot04_07[];

void func_801b25cc_slot04_07(Object *obj) {
    data_801c1f64_slot04_07[obj->state_3](obj);
    play_sound_from_step_flags_table_slot04_07(obj);
}
