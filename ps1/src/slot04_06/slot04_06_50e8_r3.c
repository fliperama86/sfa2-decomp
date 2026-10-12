/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_hit_flag_range_check_or_default_slot04_06(Object *obj) {
    obj->state_3++;
    if (obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
