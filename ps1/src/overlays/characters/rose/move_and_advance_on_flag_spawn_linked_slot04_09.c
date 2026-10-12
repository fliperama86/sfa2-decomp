/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);
void spawn_linked_pooled_object_health_2_slot04_09(Object *obj);

void move_and_advance_on_flag_spawn_linked_slot04_09(Object *obj) {
    apply_velocity_and_acceleration_slot04_09(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        spawn_linked_pooled_object_health_2_slot04_09(obj);
    }
    update_sequence(obj);
}
