/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void push_other_by_vel_x(Object *object) {
    int z;
    int limit;
    int x;
    int t;
    Object *other = object->other;
    ref_other.p = other;
    set_compare_limit_from_box_distance(object, other);
    if (object->facing_side != 0) {
        if (!(object->pos_x < ref_other.p->pos_x)) return;
    } else {
        if (!(ref_other.p->pos_x < object->pos_x)) return;
    }
    limit = 0x40;
    x = (s16)object->compare_limit;
    t = table_8017ac1c[object->kind];
    if (x > limit || x < t) return;
    if (object->field_2a0 != 0) return;
    if ((object->active_state | ref_other.p->active_state) != 0) return;
    {
        s32 delta = object->vel_x;
        if (object->facing_side != 0) delta = -delta;
        *(s32 *)&ref_other.p->pos_x_frac = delta + *(s32 *)&ref_other.p->pos_x_frac;
    }
    if (object->vel_x > 0x1000) {
        if (ref_other.p->mode_flag_2 == 0 && (z = *(s16 *)&ref_other.p->prev_input_bits_2 == 0, ref_other.p->input_bits_2 & z)) {
            object->vel_x = object->vel_x - 0x10000;
            if (object->vel_x < 0) object->vel_x = 0;
            object->accel_x = object->accel_x - 0x6000;
            if (object->accel_x < 0) {
                object->vel_x = 0;
                object->accel_x = 0;
            }
        }
        object->vel_x = object->vel_x + object->accel_x;
    }
}
