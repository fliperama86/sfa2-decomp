/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b22e4_slot04_06(Object *obj) {
    obj->vel_x = 0x40000;
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->vel_y = 0x28000;
    obj->state_3++;
    obj->accel_x = 0;
    obj->accel_y = 0xffff6000;
    obj->active_state = 1;
    if (obj->field_4b == 0) {
        obj->field_165 = 0xff;
    } else {
        obj->field_165 = 1;
    }
}
