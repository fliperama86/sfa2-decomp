/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_state_1_1_2_0_clear_action_timer(Object *object) {
    if ((short)object->health < 0) {
        enter_state_1_6_and_set_sequence(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->step_applied_flag = 0;
        object->action_timer = 0;
    }
}
