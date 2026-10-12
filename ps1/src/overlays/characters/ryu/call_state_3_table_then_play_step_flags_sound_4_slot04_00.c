/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void play_side_sound_from_step_flags_slot04_00(Object *obj);
extern ObjectFn data_801c0584_slot04_00[];

void call_state_3_table_then_play_step_flags_sound_4_slot04_00(Object *obj) {
    data_801c0584_slot04_00[obj->state_3](obj);
    play_side_sound_from_step_flags_slot04_00(obj);
}
