/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void reset_sequence_on_flag_then_advance_slot06_08(Object *obj);
void start_sequence_when_pause_flag_set_slot06_08(Object *obj);

extern SequenceStep *data_801ed480_slot06_08[];
extern SequenceStep *data_801ed498_slot06_08[];
extern SequenceStep *data_801ed4b4_slot06_08[];
extern ObjectFn data_801ed4d4_slot06_08[];

void call_update_by_table_index_zero_slot06_08(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->table_index == 0) {
            reset_sequence_on_flag_then_advance_slot06_08(obj);
            return;
        }
        start_sequence_when_pause_flag_set_slot06_08(obj);
    }
    set_flag_and_push_if_mode_8(obj);
}

void reset_sequence_on_flag_then_advance_slot06_08(Object *obj) {
    if (obj->state_1 != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_1 = 0;
            set_sequence_entry(obj, data_801ed498_slot06_08[0]);
        }
    }
    advance_sequence_checked(obj);
    set_flag_and_push_if_mode_8(obj);
}

void start_sequence_when_pause_flag_set_slot06_08(Object *obj) {
    if (obj->state_1 == 0) {
        if (game_state.pause_flag_2 != 0) {
            if (game_state.countdown_timer == 0) {
                obj->state_1++;
                set_sequence_entry(obj, data_801ed4b4_slot06_08[0]);
            }
        }
    }
    advance_sequence_checked(obj);
}

void release_object_record_2_slot06_08(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_08(Object *obj) {
    data_801ed4d4_slot06_08[obj->state_0](obj);
}

void func_801ea0cc_slot06_08(Object *obj) {
    s16 idx;
    obj->spawn_mode = 4;
    obj->state_0++;
    idx = game_state.count_index;
    if (obj->table_index != 0) {
        idx = ~idx;
    }
    if (idx & 1) {
        obj->state_0 = 2;
    }
    idx = 10;
    obj->field_0c = 0;
    obj->field_0a = 1;
    obj->field_0f = 1;
    if (obj->table_index != 0) {
        idx = 12;
    }
    set_sequence_entry(obj, data_801ed480_slot06_08[idx]);
}
