/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void advance_state_1_and_set_sequence_zero(Object *object) {
    object->state_1++;
    set_random_sequence_from_variant(object, 0);
    set_position_from_parent_table(object);
}

void update_follow_until_parent_inactive(Object *object) {
    ref_other.p = object->parent;
    if (ref_other.p->active_state == 0) {
        object->state_1++;
        set_sequence_from_table_5(object, 0x33, (SequenceStep **)table_8017c7f8);
    }
    set_position_from_parent_table(object);
    advance_sequence_checked(object);
}

void next_state_if_step_flags_negative(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_0++;
    } else {
        advance_sequence_checked(object);
    }
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_80180274 holds this function and is declared with this parameter. */
void func_80152870(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_80180274 holds this function and is declared with this parameter. */
void func_80152890(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

u8 is_linked_object_field_clear(Object *object) {
    ref_other.p = object->parent;
    return (*(u32 *)&ref_other.p->once_flag_2 & 0xffffff) == 0;
}
