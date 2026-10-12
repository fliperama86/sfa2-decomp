/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 move_ballistic_and_check_ground_slot04_0b(Object *object);

void update_airborne_until_landing_to_saved_y_slot04_0b(Object *obj) {
    int t;
    unsigned u;
    int y;

    if (move_ballistic_and_check_ground_slot04_0b(obj)) {
        if ((u8)obj->step_flags != 4) {
            update_sequence(obj);
        }
    } else {
        t = obj->state_3;
        u = obj->direction_index;
        y = ((Slot04aObj *)obj)->saved_pos_y;
        obj->active_state = 0;
        obj->variant_index_2 = 0;
        t++;
        obj->state_3 = t;
        obj->pos_y = y;
        set_sequence_by_side_and_mode(obj, (u >> 1) + 0x45);
    }
}
