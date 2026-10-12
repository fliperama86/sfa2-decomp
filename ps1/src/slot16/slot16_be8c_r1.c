/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

void set_sequence_by_side_and_mode(Object *object, int arg);
void add_to_meter_clamped(Object *object, short delta);
void reset_object_to_state_1_0_sequence_0(Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *obj);
extern s16 data_8007e878_slot16[];
void overlay_left_kind(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);
void set_sequence_and_tables_from_direction(Object *object);
void func_801b63fc(Object *object);

void set_sequence_by_direction_index_slot16(Object *object) {
    int t;

    if (object->attack_param_d != 0) {
        object->kind = 0x13;
        overlay_left_kind(object);
        select_box_tables(object);
        build_metrics(object);
        func_801b63fc(object);
        return;
    }
    if (object->limit_lo & 1) {
        object->input_dir_second = 2;
    }
    object->state_3 = 3;
    object->input_dir_0 = 4;
    object->active_flag_3 = 1;
    add_to_meter_clamped(object, *(s16 *) ((char *) data_8007e878_slot16 + (object->direction_index & 0xfe)));
    t = 0xc;
    if (object->variant_index != 0) {
        t = 0x12;
    }
    if (object->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(object, (object->direction_index >> 1) + t);
}

void set_active_flag_3_and_call_direction_sequence_slot16(Object *object) {
    object->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(object);
}

void reset_object_to_state_1_sequence_0_slot16(Object *object) {
    reset_object_to_state_1_0_sequence_0(object);
}

void load_velocities_from_table_slot16(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}
