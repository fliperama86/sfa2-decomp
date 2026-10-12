/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 apply_velocity_and_check_ground_slot04_0a(Object *obj);
void land_at_saved_pos_y_set_state_3_four_slot04_0a(Object *obj);

void update_air_motion_or_land_slot04_0a(Object *obj) {
    if (apply_velocity_and_check_ground_slot04_0a(obj)) {
        land_at_saved_pos_y_set_state_3_four_slot04_0a(obj);
    } else {
        update_sequence(obj);
    }
}
