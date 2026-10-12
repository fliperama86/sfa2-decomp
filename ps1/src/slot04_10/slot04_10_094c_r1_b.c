/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6998_slot04_10[];
extern ObjectFn data_801c69a0_slot04_10[];

void set_sequence_by_side(Object *object, int arg);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void func_80131638(Object *object);

void load_velocities_from_table_3_slot04_10(Object *obj);
void func_801b1308_slot04_10(Object *obj);
void func_801b16c8_slot04_10(Object *obj);

void call_handler_by_state_2_2_slot04_10(Object *obj) {
    data_801c6998_slot04_10[obj->state_2](obj);
}

void start_sequence_22_with_sound_slot04_10(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    set_sequence_by_side(obj, 0x22);
    play_side_sound_at(obj, obj->side, 0xb);
}

void func_801b1038_slot04_10(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    update_sequence(obj);
}

void load_velocities_from_table_slot04_10(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void load_velocities_from_table_2_slot04_10(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void load_velocities_from_table_3_slot04_10(Object *obj) {
    func_80131638(obj);
}

void call_handler_by_input_dir_or_state_3_slot04_10(Object *obj) {
    if (obj->input_dir_0 != 0) {
        func_801b16c8_slot04_10(obj);
    } else if (obj->input_dir_second != 0) {
        func_801b1308_slot04_10(obj);
    } else {
        data_801c69a0_slot04_10[obj->state_3](obj);
    }
}
