/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0744_slot04_03[];

void count_down_low_byte_then_reset_states_slot04_03(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int t;

    o->hit_count_3++;
    update_sequence(o);
    o->countdown = t = (o->countdown & 0xff00) | ((o->countdown & 0xff) - 1);
    if ((t & 0xff) == 0) {
        if (o->active_state != 0) {
            o->state_0 = 1;
        }
        o->state_2 = 3;
        o->state_1 = 0;
        o->state_3 = 1;
        func_80142c70(o);
    } else {
        if (obj->field_134 != 0) {
            if (obj->field_27d == 0) {
                obj->field_27d = o->hit_count_3;
            }
        } else if ((t & 0xff00) != 0) {
            o->countdown = t - 0x100;
        }
        push_other_by_vel_x(o);
    }
}

void call_handler_by_input_dir_0_3_slot04_03(Object *obj) {
    data_801c0744_slot04_03[obj->input_dir_0](obj);
}
