/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int rand(void);
void advance_or_pick_random_sequence_slot06_07(Object *obj);
void set_random_sequence_from_table_slot06_07(Object *obj);
Object *call_handler_by_player_kind_flags_slot06_07(void);

extern SequenceStep *data_801ee188_slot06_07[];
extern SequenceStep *data_801ee18c_slot06_07;
extern SequenceStep *data_801ee190_slot06_07;
extern u8 data_801ee1bc_slot06_07[];
extern ObjectFn data_801ee1cc_slot06_07[];
extern ObjectFn data_801ee1dc_slot06_07[];

void update_random_sequence_unless_flags_then_push_slot06_07(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        advance_or_pick_random_sequence_slot06_07(obj);
    }
    set_flag_and_push_if_mode_8(obj);
}

void advance_or_pick_random_sequence_slot06_07(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        set_random_sequence_from_table_slot06_07(obj);
    } else {
        advance_sequence_checked(obj);
    }
}

void set_random_sequence_from_table_slot06_07(Object *obj) {
    u8 v = data_801ee1bc_slot06_07[rand() & 0xf];
    set_sequence_entry(obj, data_801ee188_slot06_07[v]);
}

void release_object_record_2_slot06_07(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_07(Object *obj) {
    data_801ee1cc_slot06_07[obj->state_0](obj);
}

void init_object_with_fixed_sequence_a_slot06_07(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    obj->state_0++;
    set_sequence_entry(obj, data_801ee18c_slot06_07);
}

void call_handler_by_state_1_and_advance_slot06_07(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801ee1dc_slot06_07[obj->state_1](obj);
        advance_sequence_checked(obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void start_sequence_when_target_is_left_slot06_07(Object *obj) {
    Object *p = call_handler_by_player_kind_flags_slot06_07();
    if (p != 0) {
        if ((u16)p->pos_x < (u16)(obj->pos_x - 0x20)) {
            obj->state_1++;
            set_sequence_entry(obj, data_801ee190_slot06_07);
        }
    }
}

void advance_state_1_random_countdown_on_flag_slot06_07(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1++;
        obj->countdown = get_random_byte();
    }
}

void count_down_then_reset_state_1_slot06_07(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->state_1 = 0;
    }
}

void release_object_record_3_slot06_07(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
