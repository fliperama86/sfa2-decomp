/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7be0_slot04_11[];

void reset_object_to_state_1_0_0_2_sequence_1(Object *obj);
void func_80131638(Object *obj);
void set_position_above_other_by_kind_slot04_11(Object *obj);
void reset_countdown_and_set_indicator_boxes_slot04_11(Object *obj);
void func_801b6f28_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);

void func_801b66ac_slot04_11(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void func_801b66cc_slot04_11(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void func_801b66ec_slot04_11(Object *obj) {
    func_80131638(obj);
}

void call_handler_by_state_0_with_other_slot04_11(Object *obj) {
    ref_other.p = obj->other;
    data_801c7be0_slot04_11[obj->state_0](obj);
}

void func_801b675c_slot04_11(Object *obj) {
    Object *o;

    obj->field_01 = 1;
    obj->data_ptr_a = data_80172a48;
    obj->field_90 = (void *)0x800fb100;
    obj->state_0++;
    obj->data_ptr_b = data_80173c9c;
    set_position_above_other_by_kind_slot04_11(obj);
    o = ref_other.p->other;
    play_side_sound_at(o, o->side, 0xf);
    ((Slot04bObj *)obj)->field_47 = ref_other.p->field_28c;
    reset_countdown_and_set_indicator_boxes_slot04_11(obj);
    func_801b6f28_slot04_11(obj);
}
