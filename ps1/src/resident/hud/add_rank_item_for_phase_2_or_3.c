/* Reconstruction. Names/roles inferred, not original symbols. */
/* queue_name_text: the unused 16-byte local reproduces the original 0x28 frame. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Form found by automatic permutation search, then cleaned by hand. */
void add_rank_item_for_phase_2_or_3(GameState *state, Object *object) {
    u8 index;

    if (object->score_rank + object->field_121 < 12) {
        if ((u8)(object->phase_index - 2) < 2) {
            if (object->phase_index != 3 || (state->loop_counter & 1)) {
                index = object->min_rank;
                index = index - 1;
                if (object->side != 0) {
                    index = index + 5;
                }
                add_item_to_table(table_801817e0[index]);
            }
        }
    }
}
