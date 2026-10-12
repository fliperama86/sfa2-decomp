/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6a54_slot04_10[];
extern ObjectRef data_80190468;

void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void start_action_if_buttons_pressed(Object *object);
void load_velocities_from_table_slot04_10(Object *obj);
void update_sequence_or_end_move_a_slot04_10(Object *obj);
void add_flag_indexed_offset_to_pos_x_slot04_10(Object *obj);

void update_sequence_or_end_move_a_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        load_velocities_from_table_slot04_10(obj);
    }
}

void call_table_function_by_state_3_13_slot04_10(Object *obj) {
    data_801c6a54_slot04_10[obj->state_3](obj);
}

void start_state_3_sequence_by_direction_slot04_10(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    ((Slot04bObj *)obj)->field_298 = 0;
    obj->field_225 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 7);
    add_one_to_ec4_if_object_unflagged((GameState *)data_80190468.p, obj);
    a = 0x49;
    if (obj->field_49 != 0) {
        a = 0x53;
    }
    set_sequence_by_side_and_mode(obj, a + (obj->direction_index >> 1));
    add_flag_indexed_offset_to_pos_x_slot04_10(obj);
}
