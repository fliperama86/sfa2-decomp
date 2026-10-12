/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);
void func_801b2d6c_slot04_09(Object *o);

/* The local t holds the facing_side read for the later p->facing_flag store: written with the field read in place, this function differs from the original in 25 instruction slots. */
void reset_other_and_reverse_on_step_flag_slot04_09(Object *obj) {
    Object *p;
    int t;

    func_801b2d6c_slot04_09(obj);
    apply_velocity_and_acceleration_slot04_09(obj);
    if (obj->step_flags & 1) {
        obj->state_3++;
        p = obj->other;
        p->state_0 = 1;
        p->state_1 = 1;
        p->state_2 = 0;
        p->state_3 = 0;
        p->vec_table_index = 0;
        p->field_60 = 0;
        p->hit_type = 1;
        p->field_62 = 3;
        p->status_flag = 0;
        obj->status_flag = 0;
        t = obj->facing_side;
        p->field_15b = 1;
        p->hit_flag = 1;
        p->field_6b = 0;
        p->active_state = 0xff;
        p->facing_flag = t ^ 1;
        p->pos_y = obj->ground_y;
        p->pos_y_frac = 0;
        p->field_261 = 0;
        p->field_27a = 0;
        obj->field_292 = 0x1e;
        p->field_292 = 0x1e;
        obj->vel_y += 0x3c000;
        obj->vel_x <<= 1;
        obj->facing_side ^= 1;
        p->facing_side = obj->facing_side;
        set_sequence_by_side_and_mode(obj, 0x23);
    } else {
        update_sequence(obj);
    }
}
