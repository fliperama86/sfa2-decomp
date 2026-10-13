/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

#define SEL (*(int *)&game_state.field_354)

void check_opposite_player_same_selection_slot27(void) {
    ObjectRef *ref;
    int *sel = (int *)scratch_word_a;
    int v;

    v = game_state.other_object->side;
    v ^= 1;
    *sel = v;
    if (((game_state.mode >> v) & 1) == 0) {
        goto zero;
    }
    {
        Object *p;

        ref = &ref_aux;
        p = &player_left;
        ref->p = p;
        if (v != 0) {
            ref->p = p + 1;
        }
    }
    v = game_state.other_object->kind;
    *sel = v;
    if (v == 0x11 || v == 0x13) {
        goto both;
    }
    if (ref_aux.p->kind == v) {
        goto same;
    }
    goto zero;
both:
    v = ref_aux.p->kind;
    SEL = v;
    if (v == 0x11) {
        goto same;
    }
    if (v != 0x13) {
        goto zero;
    }
same:
    {
        int *t = (int *)scratch_word_a;

        v = game_state.other_object->field_d4;
        *t = v;
        if (ref_aux.p->field_d4 != v) {
            goto zero;
        }
        *t = 1;
        return;
    }
zero:
    SEL = 0;
}
