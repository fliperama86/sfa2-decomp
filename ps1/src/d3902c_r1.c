/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/*
 * What it does (inferred, not an original name): collision push-out between
 * the two players. It tests the boxes of player_left and player_right for
 * overlap, first in y, then in x. If they overlap in both, it adds the x
 * overlap, as a 16.16 amount, to the word at offset 0x10 of one player or,
 * halved and in opposite directions, of both. Which one moves depends on
 * field_164 and field_45 of each player.
 */
void func_8013902c(void) {
    Object *o1;
    Object *o2;
    Box6 *b1;
    Box6 *b2;
    u8 i1;
    u8 i2;
    ObjectRef left_ref;
    Object *target;
    int far;
    int dist;
    int tmp;

    /* a plain "ref_third.p = &player_left;" differs in 63 instruction lines */
    left_ref.p = &player_left;
    ref_third = left_ref;
    game_state.other_object = &player_left + 1;
    o1 = ref_third.p;
    i1 = o1->frame->box_index;
    if (i1 == 0)
        return;
    o2 = ref_other.p;
    i2 = o2->frame->box_index;
    if (i2 == 0)
        return;
    b1 = (Box6 *)o1->unknown_148 + i1;
    b2 = (Box6 *)o2->unknown_148 + i2;
    /* without the (s16) casts on origin_y each differs in 1 instruction line (lh against lhu) */
    tmp = (s16)b1->origin_y;
    dist = (s16)b2->origin_y;
    tmp -= o1->pos_y;
    dist -= o2->pos_y;
    dist -= tmp;
    if (dist < 0)
        dist = -dist;
    /* dist and tmp are used again from here for other values, and the sum is built in two
       steps: written in one statement (tmp = b1->field_05 + b2->field_05;) this function
       differs from the original in 2 instruction lines, and as
       dist -= b1->field_05 + b2->field_05; in 4 */
    tmp = b1->field_05;
    tmp += b2->field_05;
    dist -= tmp;
    if (dist >= 0)
        return;
    tmp = b1->origin;
    if (o1->field_0b != 0)
        tmp = -tmp;
    tmp += o1->pos_x;
    dist = b2->origin;
    if (o2->field_0b != 0)
        dist = -dist;
    dist += o2->pos_x;
    dist -= tmp;
    far = 0;
    if (dist < 0) {
        dist = -dist;
        far = 1;
    }
    tmp = b1->extent;
    tmp += b2->extent;
    dist -= tmp;
    if (dist >= 0)
        return;
    dist = -dist;
    /* a rotate by 16 (the two halves swap places); without the (u32) casts it differs in 1
       instruction line, with | in place of + in 3 */
    dist = ((u32)dist >> 16) + ((u32)dist << 16);
    /* The goto tail is needed: written as nested if/else with the code in each arm it differs in
       138 instruction lines, with merged conditions in 134, with one add at the end in 130; the
       three labels used once, inlined one at a time, differ in 22 to 41. */
    if (o1->field_164 != 0)
        goto o1_set;
    if (o2->field_164 != 0)
        goto move_left;
    if (o1->field_45 != 0)
        goto o1_still;
    if (o2->field_45 != 0)
        goto o2_only;
both:
    dist >>= 1;
    tmp = -dist;
    if (far && !(ref_third.p->pos_x < ref_other.p->pos_x)) {
        tmp = dist;
        dist = -dist;
    }
    /* a word write: the word at offset 0x10 holds pos_x_frac and the field after it */
    *(s32 *)&ref_third.p->pos_x_frac += tmp;
    target = ref_other.p;
    goto add;
o1_still:
    if (o2->field_45 != 0)
        goto both;
    if (far == 0)
        dist = -dist;
    *(s32 *)&o1->pos_x_frac += dist;
    return;
o2_only:
    if (far)
        dist = -dist;
    *(s32 *)&o2->pos_x_frac += dist;
    return;
o1_set:
    if (o2->field_164 != 0)
        goto both_set;
move_right:
    if (ref_third.p->field_164 != 1)
        dist = -dist;
    target = game_state.other_object;
    goto add;
both_set:
    if (o1->field_45 != 0)
        goto move_left;
    if (o2->field_45 != 0)
        goto move_right;
move_left:
    if (ref_other.p->field_164 != 1)
        dist = -dist;
    target = ref_third.p;
add:
    *(s32 *)&target->pos_x_frac += dist;
}
