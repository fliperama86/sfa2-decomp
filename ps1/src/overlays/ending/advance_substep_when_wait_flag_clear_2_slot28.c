/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_8002fce8_slot28[];
extern Object *data_800518fc_slot28[];

void advance_substep_when_wait_flag_clear_2_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        screen_state->substep_index += 1;
    }
}

void set_sequence_of_fourth_hud_object_3_slot28(Object *obj, int arg) {
    set_sequence_from_table_5(data_800518fc_slot28[3], arg, data_8002fce8_slot28);
}
