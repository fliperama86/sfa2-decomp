/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *obj);

void func_801b51e0_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p = o->other;
    u8 *c;

    o->state_3++;
    c = (u8 *)obj->linked_object;
    c[4] = 2;
    c[5] = 0;
    c[6] = 0;
    c[7] = 0;
    p->pos_y = p->ground_y;
    p->field_0c = obj->field_334;
    p->image_row = obj->field_335;
    o->vel_x = 0x40000;
    o->vel_y = 0;
    o->accel_x = 0;
    o->accel_y = -0x6000;
    if (o->facing_side != 0) {
        o->vel_x = 0x40000;
    } else {
        o->vel_x = 0xfffc0000;
    }
    p->facing_side = p->facing_side ^ 1;
    set_sequence_by_side_and_mode(o, 0x1a);
}

void update_fall_until_ground_then_sequence_slot04_09(Object *o, Object *unused) {
    apply_velocity_and_acceleration_slot04_09(o);
    if (o->pos_y < o->ground_y) {
        update_sequence(o);
    } else {
        o->state_3++;
        o->pos_y_frac = 0;
        o->pos_y = o->ground_y;
        set_sequence_by_side_and_mode(o, 0x1b);
    }
}

void finish_sequence_flip_facing_and_reset_slot04_09(Object *obj, Object *unused) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->facing_side = obj->facing_side ^ 1;
        obj->status_flag = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
