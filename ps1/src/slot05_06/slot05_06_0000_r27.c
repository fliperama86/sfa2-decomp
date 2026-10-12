/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ca2e4_slot05_06(Object *obj) {
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

void func_801ca350_slot05_06(Object *obj) {
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->accel_x = 0;
    obj->state_3++;
    obj->vel_x = 0x20000;
    obj->vel_y = 0x78000;
    obj->accel_y = -0x6000;
    set_sequence_by_side_and_mode(obj, 0x1f);
}

void wait_countdown_2_then_advance_on_mode_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->countdown_2 != 0) {
        obj->countdown_2--;
    } else {
        if (obj->mode != 0) {
            o->active_state = 1;
            obj->field_1c8 = 0;
            o->state_3++;
        }
        update_sequence(o);
    }
}
