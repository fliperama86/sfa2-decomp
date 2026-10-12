/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8007ef24_slot2b;

void fall_to_ground_then_set_bounce_velocity_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->pos_y_hi -= s->vel_y_hi;
    s->vel_y_hi += s->accel_y_hi;
    if (data_8007ef24_slot2b <= o->pos_y) {
        o->state_1++;
        o->pos_y_frac = 0;
        o->vel_y = 0x20000;
        o->accel_y = -0x4000;
        o->pos_y = data_8007ef24_slot2b;
    }
    advance_sequence_checked(o);
}

void fall_to_ground_then_set_state_0_three_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->pos_y_hi -= s->vel_y_hi;
    s->vel_y_hi += s->accel_y_hi;
    if (data_8007ef24_slot2b <= o->pos_y) {
        o->state_0 = 3;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->pos_y_frac = 0;
    }
    advance_sequence_checked(o);
}

void release_object_and_unlink_from_parent_slot2b(Object *o) {
    Object *p = o->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}
