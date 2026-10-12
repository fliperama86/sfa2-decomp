/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

/*
 * What it does (inferred, not an original name): places a follower object
 * relative to a target. The object's parent points at a parent; the
 * parent's `other` is the target. From the table of box records of the
 * parent's side (the left table in scratchpad when the parent's side is 0,
 * the right one otherwise), selected by the target's kind, it takes the
 * 6-byte record numbered by the parent's current frame's field_0b. The
 * object copies the target's field_0b; its pos_x becomes the target's
 * pos_x plus the sum of the record's first half and the object's offset_x,
 * that sum (not the target's pos_x) negated when the target's field_0b is
 * not 0; its pos_y becomes the target's pos_y minus the sum of the
 * record's second half and the high half of the object's vel_y. All
 * position arithmetic is modulo 2^16.
 */
void place_object_at_target_box_offset_slot2b(Object *obj) {
    Object *parent = obj->parent;
    Object *target = parent->other;
    u16 *e;
    short x;
    short t;
    u32 m;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    /* Without it this function differs from the original in 5 instruction slots. */
    u8 unused[8];
    int y;

    if (parent->side == 0) {
        e = (u16 *)boxes_74_left[target->kind];
    } else {
        e = (u16 *)boxes_124_right[target->kind];
    }
    e += parent->frame->box_table_index * 3;
    x = e[0];
    t = x + ((Slot2bObj *)obj)->offset_x;
    y = e[1];
    obj->facing_side = target->facing_side;
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* Without it (the use written as (u32)obj->vel_y >> 16) this function differs from the original in 8 instruction slots. */
    m = 0xffff0000;
    x = t;
    if (target->facing_side != 0) {
        x = -t;
    }
    obj->pos_x = x + (u16)target->pos_x;
    y = (u16)target->pos_y - (y + ((obj->vel_y & m) >> 16));
    obj->pos_y = y;
}
