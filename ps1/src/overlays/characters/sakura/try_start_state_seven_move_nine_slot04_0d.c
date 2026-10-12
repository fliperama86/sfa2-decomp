/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void advance_state_3_start_table_sequence_slot04_0d(Object *obj);

extern ObjectFnInt data_801c2980_slot04_0d[];
extern ObjectFn data_801c29a8_slot04_0d[];
extern ObjectFn data_801c29d0_slot04_0d[];
extern ObjectFn data_801c29f8_slot04_0d[];
extern ObjectFn data_801c2a08_slot04_0d[];
extern u8 data_801c2764_slot04_0d[];

int try_start_state_seven_move_nine_slot04_0d(Object *obj) {
    if (obj->field_7e != 0 || obj->remaining_count != 0) {
        if (try_action_clearing_both_flags(obj) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->field_15a = 9;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            return 1;
        }
    }
    return 0;
}

void func_801b1158_slot04_0d(Object *obj) {
    data_801ad398 = data_801c2980_slot04_0d[obj->field_15a](obj);
}

int is_linked_object_3_null_slot04_0d(Object *obj) {
    return obj->linked_object_3 == 0;
}

int is_meter_at_least_threshold_slot04_0d(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int return_true_always_slot04_0d(Object *obj) {
    return 1;
}

int has_remaining_count_slot04_0d(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b11d4_slot04_0d(Object *obj) {
    data_801c29a8_slot04_0d[obj->field_15a](obj);
}

void func_801b1214_slot04_0d(Object *obj) {
    data_801c29d0_slot04_0d[obj->field_15a](obj);
}

void call_table_function_by_state_3_4_slot04_0d(Object *obj) {
    data_801c29f8_slot04_0d[obj->state_3](obj);
}

void update_sequence_chain_next_table_sequence_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    if ((s16)o->step_flags >= 0) {
        if (o->direction_index != 0) {
            obj->active_flag_2 = 1;
        }
    } else {
        if (obj->active_flag_2 == 0 || (obj->active_flag_2 = 0, obj->table_index_3 == 2)) {
            advance_state_3_start_table_sequence_slot04_0d(o);
        } else {
            obj->table_index_3 = obj->table_index_3 + 1;
            o->direction_index = o->direction_index - 1;
            add_to_meter_clamped(o, 1);
            set_sequence_by_side_and_mode(o, (u8)(data_801c2764_slot04_0d[obj->table_index_3] + 0x28));
        }
    }
}

void advance_state_3_start_table_sequence_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 t;

    o->state_3++;
    t = data_801c2764_slot04_0d[obj->table_index_3];
    o->field_225 = 1;
    o->direction_index = t * 2;
    set_sequence_by_side_and_mode(o, (u8)(t + 0x2b));
}

void call_table_function_by_state_3_5_slot04_0d(Object *obj) {
    data_801c2a08_slot04_0d[obj->state_3](obj);
}

void start_chained_sequence_state_3_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    obj->active_flag_2 = 0;
    obj->table_index_3 = 0;
    set_sequence_by_side_and_mode(o, 0x27);
}
