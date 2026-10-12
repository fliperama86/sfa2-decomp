/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void switch_sequence_on_player_state_2_eight_slot06_08(Object *obj);

extern SequenceStep *data_801ed480_slot06_08[];

/* One local holds the OR of the two flags and later the table index.
   Written as one expression for the OR, or with an index local of its own,
   this function differs from the original in seven instruction slots. */
void update_object_state_1_sequence_switch_slot06_08(Object *obj) {
    int v = game_state.field_65;
    v |= game_state.field_74;
    if (v == 0) {
        if (obj->state_1 != 0) {
            if ((s16)obj->step_flags & 0x8000) {
                v = 10;
                obj->state_1 = 0;
                if (obj->table_index != 0) {
                    v = 12;
                }
                set_sequence_entry(obj, data_801ed480_slot06_08[v]);
            }
            advance_sequence_checked(obj);
        } else {
            switch_sequence_on_player_state_2_eight_slot06_08(obj);
        }
    }
    set_flag_and_push_if_mode_8(obj);
}
