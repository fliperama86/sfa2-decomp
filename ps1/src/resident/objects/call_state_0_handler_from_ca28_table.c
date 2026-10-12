/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_state_0_handler_from_ca28_table(Block172 *block) {
    Object *object = (Object *)block;
    fn_table_8017ca28[object->state_0](object);
}

void init_object_spawn_position_and_sequence(Object *object) {
    object->spawn_x = 0x60;
    object->spawn_y = 0x1e0;
    object->field_90 = (void *)0x800fb100;
    object->data_ptr_a = data_80172a48;
    object->data_ptr_b = data_80173c9c;
    object->state_0 = object->state_0 + 1;
    set_sequence_from_table_5(object, object->variant_index, (SequenceStep **)table_8017c7f8);
}

/* The call of set_flag_and_push_if_mode_8 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 15 instruction slots. */
void func_80146794(Object *object) {
    if (object->table_index >= 0x10) {
        if (object->table_index == 0x10) {
            object->pos_x = object->pos_x + 4;
        } else if (object->table_index == 0x11) {
            object->pos_x = object->pos_x - 4;
        }
    }
    if ((s16)object->step_flags < 0) {
        object->state_0 = object->state_0 + 1;
    }
    advance_sequence_checked(object);
    ((void (*)(void))set_flag_and_push_if_mode_8)();
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fn_table_8017ca28 holds this function and is declared with this parameter. */
void push_object_to_pool_table_wrapper_6(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

u8 spawn_random_offset_effect_index_zero(Object *object) {
    return spawn_effect_record_with_random_offset(object, 0);
}

u8 spawn_random_offset_effect_index_one(Object *object) {
    return spawn_effect_record_with_random_offset(object, 1);
}

int spawn_random_offset_effect_index_two(Object *object) {
    u8 r = spawn_effect_record_with_random_offset(object, 2);
    return r;
}

u8 spawn_random_offset_effect_index_four(Object *object) {
    return spawn_effect_record_with_random_offset(object, 4);
}

u8 spawn_random_offset_effect_index_five(Object *object) {
    return spawn_effect_record_with_random_offset(object, 5);
}

int spawn_table_effect_at_object_index_two(Object *object) {
    u8 r = spawn_effect_from_table_at_object(object, 2);
    return r;
}

u8 spawn_table_effect_at_object_index_four(Object *object) {
    return spawn_effect_from_table_at_object(object, 4);
}

int spawn_object_variant_5_from_object(Object *object) {
    u8 r = spawn_effect_from_table_at_object(object, 5);
    return r;
}

void spawn_two_effects_at_other_ground(Object *object) {
    spawn_effect_at_other_ground(object, 0);
    spawn_effect_at_other_ground(object, 1);
}
