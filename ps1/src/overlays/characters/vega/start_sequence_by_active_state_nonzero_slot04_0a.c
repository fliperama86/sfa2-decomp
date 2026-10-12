/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_by_active_state_nonzero_slot04_0a(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->active_state != 0) {
        set_sequence_by_side(obj, 0x2f);
    } else {
        set_sequence_by_side(obj, 0x2b);
    }
}
