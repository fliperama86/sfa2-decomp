/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void count_down_timer_set_side_bits_slot04_06(Object *obj) {
    if (((Slot04aObj *)obj)->timer != 0) {
        ((Slot04aObj *)obj)->timer--;
        if (((Slot04aObj *)obj)->timer == 0) {
            game_state_second.side_bits = game_state.side_bits_2 | (1 << obj->side);
        }
    }
    update_sequence(obj);
}
