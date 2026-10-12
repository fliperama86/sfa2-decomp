/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred, not original names): for an object whose field_49
   is non-zero it returns func_8013f474(object, a, b) as a byte when its
   halfword at offset 4 is 1 and state_2 is 7, 0 when the halfword is 1 and
   state_2 is not 7, and 1 when the halfword is not 1. Otherwise, with the
   other object (ref_other) in mode 1 and with a box, it compares two x
   positions: the object's pos_x plus a (minus a when facing_side is set), and
   game_state.other_object's pos_x plus the box's origin minus its extent
   (negated when that object's facing_side is set). It returns
   func_8013fab4(object) as a byte when they are at most b apart; b grows by
   4 when hold_timer is set, unless the object's halfword at offset 4 is 1
   and its state_2 is 7 or 8. */
int func_8013f8c4(Object *object, int a_arg, int b_arg) {
    s16 a = a_arg; /* the shared prototype takes int */
    s16 b = b_arg;
    FrameRecord *frame;
    Box6 *box;
    s16 half_width, distance;
    u16 mode;
    if (object->field_49 != 0) {
        if (*(u16 *)&object->state_0 == 1) {
            if (object->state_2 == 7)
                return func_8013f474(object, a, b) & 0xff;
            return 0;
        }
    } else {
        ref_other.p = object->other;
        if (ref_other.p->other_state_code != 0)
            return 0;
        if (ref_other.p->other_lock != 0)
            return 0;
        mode = *(u16 *)&ref_other.p->state_0;
        if (mode != 1)
            return 0;
        if (game_state.config->mode_flag_c != 0)
            return 0;
        if (ref_other.p->active_state != 0)
            return 0;
        frame = ref_other.p->frame;
        if ((frame->box_a | frame->box_b | frame->box_c) == 0)
            return 0;
        if (object->facing_side != 0)
            a = -a;
        a = a + object->pos_x;
        box = &((Box6 *)ref_other.p->unknown_148)[frame->box_index];
        half_width = box->origin - box->extent;
        if (*(u16 *)&object->state_0 == mode) {
            if (object->state_2 == 7)
                goto keep;
            if (object->state_2 == 8)
                goto keep;
        }
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        do {
            if (object->hold_timer != 0)
                b = b + 4;
        } while (0);
    keep:
        if (game_state.other_object->facing_side != 0)
            half_width = -half_width;
        distance = half_width + game_state.other_object->pos_x;
        a = a - distance;
        if (a < 0)
            a = -a;
        if (b < a)
            return 0;
        return func_8013fab4(object) & 0xff;
    }
    /* The path with field_49 set and the halfword not 1 ends here. The original has no instruction for this result: it branches to the function's end with the 1 of the comparison still in the result register. The compiler adds none for this line either. Without the line the PS1 code is the same and the result is undefined in C. */
    return 1;
}
