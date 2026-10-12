/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_sequence_table_and_start_entry_slot06_0f(Object *obj);

extern ObjectFn data_801eb4d8_slot06_0f[];
extern SequenceStep **data_801eb4e0_slot06_0f[];
extern Slot06_0fRecb4e4 data_801eb4e4_slot06_0f[];

void call_handler_by_state_1_and_advance_slot06_0f(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801eb4d8_slot06_0f[obj->state_1](obj);
        advance_sequence_checked(obj);
    }
    set_flag_and_push_if_mode_8(obj);
}

void start_sequence_when_countdown_done_slot06_0f(Object *obj) {
    if (game_state.pause_flag_2 != 0 && game_state.countdown_timer == 0) {
        Slot06Obj *s = (Slot06Obj *)obj;
        obj->state_1++;
        set_sequence_entry(obj, s->sequence_table[obj->variant_index]);
    }
}

void noop_slot00_slot06_0f(Object *obj) {
}

void release_object_record_2_slot06_0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_sequence_table_and_start_entry_slot06_0f(Object *obj) {
    Slot06Obj *s = (Slot06Obj *)obj;
    s->sequence_table = data_801eb4e0_slot06_0f[obj->table_index];
    obj->variant_index = data_801eb4e4_slot06_0f[obj->table_index].entry_value;
    set_sequence_entry(obj, s->sequence_table[data_801eb4e4_slot06_0f[obj->table_index].entry_index]);
}
