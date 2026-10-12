/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void play_sound_from_step_flags_table_slot04_07(Object *obj);
extern ObjectFn data_801c20f8_slot04_07[];

void call_state_3_handler_then_play_step_sound_6_slot04_07(Object *obj) {
    data_801c20f8_slot04_07[obj->state_3](obj);
    play_sound_from_step_flags_table_slot04_07(obj);
}
