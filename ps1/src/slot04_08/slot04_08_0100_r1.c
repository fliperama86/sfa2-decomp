/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3b38_slot04_08[];
void play_sound_on_step_flag_high_byte_3_slot04_08(Object *o);

void call_state_2_handler_then_play_step_sound_slot04_08(Object *obj) {
    data_801c3b38_slot04_08[obj->state_2](obj);
    play_sound_on_step_flag_high_byte_3_slot04_08(obj);
}
