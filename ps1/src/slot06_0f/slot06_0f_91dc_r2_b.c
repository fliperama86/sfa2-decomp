/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_object_by_table_index_handler_slot06_0f(Object *obj);

extern SequenceStep *data_801ecba4_slot06_0f[];
extern ObjectFn data_801eb4b0_slot06_0f[];
extern ObjectFn data_801eb4b8_slot06_0f[];
extern ObjectFn data_801eb4c0_slot06_0f[];
extern ObjectFn data_801eb4c8_slot06_0f[];

void update_object_by_table_index_handler_slot06_0f(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801eb4b0_slot06_0f[obj->table_index](obj);
        advance_sequence_checked(obj);
    }
    func_8011ffdc(obj);
}

void call_handler_by_state_1_slot06_0f(Object *obj) {
    data_801eb4b8_slot06_0f[obj->state_1](obj);
}

void advance_state_1_on_random_chance_slot06_0f(Object *obj) {
    if ((get_random_byte() & 3) == 0) {
        obj->state_1++;
        set_sequence_entry(obj, data_801ecba4_slot06_0f[3]);
    }
}

void reset_state_1_if_step_flags_negative_slot06_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_1 = 0;
    }
}

void call_handler_by_state_1_2_slot06_0f(Object *obj) {
    data_801eb4c0_slot06_0f[obj->state_1](obj);
}

void advance_state_1_and_set_sequence_5_slot06_0f(Object *obj) {
    obj->state_1++;
    set_sequence_entry(obj, data_801ecba4_slot06_0f[5]);
}

void reset_state_1_and_set_sequence_4_on_loop_slot06_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        return;
    }
    obj->state_1 = 0;
    set_sequence_entry(obj, data_801ecba4_slot06_0f[4]);
}

void release_object_record_slot06_0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_2_slot06_0f(Object *obj) {
    data_801eb4c8_slot06_0f[obj->state_0](obj);
}
