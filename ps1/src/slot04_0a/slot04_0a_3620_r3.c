/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_hit_count_countdown_enter_state_slot04_0a(Object *o) {
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
        set_side_flag(o);
        init_object_after_hit_and_spawn_three_effects(o);
    } else {
        if (obj->field_134 != 0) {
            if (obj->field_27d == 0) {
                obj->field_27d = o->hit_count_3;
            }
        } else if ((t & 0xff00) == 0) {
            set_side_flag(o);
        } else {
            o->countdown = t - 0x100;
            if ((o->countdown & 0x100) == 0) {
                set_side_flag(o);
            }
        }
        push_other_by_vel_x(o);
    }
}
