/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea940_slot06_01[];
extern ObjectFn data_801ea94c_slot06_01[];
extern SequenceStep *data_801eb990_slot06_01;
extern SequenceStep *data_801eb994_slot06_01;

void count_down_random_start_sequence_slot06_01(Object *obj);
void call_handler_for_both_players_slot06_01(Object *obj);
void set_state_1_if_other_state_2_is_7_or_8_slot06_01(Object *a, Object *b);

void call_handler_by_state_1_table_a_slot06_01(Object *o) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801ea940_slot06_01[o->state_1](o);
    }
    func_8011ffdc(o);
}

void func_801e97c4_slot06_01(Object *obj) {
    count_down_random_start_sequence_slot06_01(obj);
    call_handler_for_both_players_slot06_01(obj);
}

void reset_state_1_at_sequence_end_slot06_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 0;
    }
    advance_sequence_checked(obj);
}

void count_down_random_start_sequence_slot06_01(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->countdown = 0x40;
        if ((get_random_byte() & 7) == 0) {
            obj->state_1 = 1;
            set_sequence_entry(obj, data_801eb990_slot06_01);
        }
    }
}

void call_handler_for_both_players_slot06_01(Object *obj) {
    Object *pair = &player_left;

    set_state_1_if_other_state_2_is_7_or_8_slot06_01(pair, obj);
    set_state_1_if_other_state_2_is_7_or_8_slot06_01(pair + 1, obj);
}

void set_state_1_if_other_state_2_is_7_or_8_slot06_01(Object *a, Object *b) {
    if ((u8)(a->state_2 - 7) < 2) {
        b->state_1 = 2;
        set_sequence_entry(b, data_801eb994_slot06_01);
    }
}

void release_object_record_slot06_01(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_2_slot06_01(Object *obj) {
    data_801ea94c_slot06_01[obj->state_0](obj);
}
