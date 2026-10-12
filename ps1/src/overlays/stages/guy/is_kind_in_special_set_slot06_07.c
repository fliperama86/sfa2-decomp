/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 is_kind_in_special_set_slot06_07(Object *obj);
void set_sequence_table_and_start_entry_slot06_07(Object *obj);

extern SequenceStep *data_801ee1a8_slot06_07;
extern ObjectFn data_801ee1f8_slot06_07[];
extern ObjectFn data_801ee208_slot06_07[];
extern SequenceStep **data_801ee210_slot06_07[];
extern Slot06_07Rece218 data_801ee218_slot06_07[];
extern ObjectFn data_801ee21c_slot06_07[];

u8 is_kind_in_special_set_slot06_07(Object *obj) {
    int r = 0;
    u8 k = obj->kind;
    if (k == 4 || k == 9 || k == 0xd || k == 0x12) {
        r = 1;
    }
    return r;
}

Object *get_null_object_slot06_07(void) {
    return 0;
}

Object *get_player_left_slot06_07(void) {
    return &player_left;
}

Object *get_player_right_slot06_07(void) {
    return &player_right;
}

Object *get_player_with_lower_pos_x_slot06_07(void) {
    Object *o = &player_right;
    if (player_right.pos_x > player_left.pos_x) {
        o = o - 1;
    }
    return o;
}

void call_handler_by_state_0_4_slot06_07(Object *obj) {
    data_801ee1f8_slot06_07[obj->state_0](obj);
}

void init_object_spawn_and_set_sequence_slot06_07(Object *obj) {
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->field_0f = 1;
    if (obj->facing_side == 0) {
        obj->field_0a = 1;
    }
    set_sequence_table_and_start_entry_slot06_07(obj);
}

void call_handler_by_state_1_and_advance_2_slot06_07(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801ee208_slot06_07[obj->state_1](obj);
        advance_sequence_checked(obj);
    }
    set_flag_and_push_if_mode_8(obj);
}

void start_sequence_when_countdown_done_slot06_07(Object *obj) {
    if (game_state.pause_flag_2 != 0 && game_state.countdown_timer == 0) {
        Slot06Obj *s = (Slot06Obj *)obj;
        obj->state_1++;
        set_sequence_entry(obj, s->sequence_table[obj->variant_index]);
    }
}

void noop_slot00_slot06_07(Object *obj) {
}

void release_object_record_4_slot06_07(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_sequence_table_and_start_entry_slot06_07(Object *obj) {
    Slot06Obj *s = (Slot06Obj *)obj;
    s->sequence_table = data_801ee210_slot06_07[obj->table_index];
    obj->variant_index = data_801ee218_slot06_07[obj->table_index].entry_value;
    set_sequence_entry(obj, s->sequence_table[data_801ee218_slot06_07[obj->table_index].entry_index]);
}

void call_handler_by_state_0_5_slot06_07(Object *obj) {
    data_801ee21c_slot06_07[obj->state_0](obj);
}

void init_object_state_0_to_1_set_sequence_slot06_07(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->state_0 = 1;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    set_sequence_entry(obj, data_801ee1a8_slot06_07);
}

void load_velocities_from_table_slot06_07(Object *obj) {
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_5_slot06_07(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
