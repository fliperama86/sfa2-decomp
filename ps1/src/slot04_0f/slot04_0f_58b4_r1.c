/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801c6144_slot04_0f[];
extern FrameRecord data_801c6150_slot04_0f[];
extern BoxTables data_801c618c_slot04_0f[];
extern ObjectFn data_801c62a4_slot04_0f[];
extern ObjectRef data_80190458;

void reset_and_push_block_object_type_8(Slab172 *o);
void func_8011ffdc(Object *o);
void call_handler_by_state_1_unless_paused_slot04_0f(Object *obj);
void advance_or_reset_states_on_frame_param_slot04_0f(Object *obj);
void func_801b5b4c_slot04_0f(Object *obj);

void init_child_object_with_tables_slot04_0f(Object *obj) {
    obj->ot_depth = 0;
    obj->variant_index_2 = 0xff;
    obj->state_0 = obj->state_0 + 1;
    obj->field_49 = data_80190458.p->field_49;
    obj->field_1c = data_80190458.p->field_1c;
    obj->box_tables = data_801c618c_slot04_0f;
    obj->health = 0xff;
    obj->active_state = 0;
    obj->frames = data_801c6150_slot04_0f;
    set_sequence_from_table_5(obj, obj->vel_table_index >> 1, data_801c6144_slot04_0f);
    obj->variant_index = 0x16;
    call_handler_by_state_1_unless_paused_slot04_0f(obj);
}

void call_handler_by_state_1_unless_paused_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        data_801c62a4_slot04_0f[obj->state_1](obj);
    }
    func_8011ffdc(obj);
}

void advance_state_0_on_frame_param_zero_slot04_0f(Object *obj) {
    if (data_80190458.p->frame->frame_param == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
    func_801b5b4c_slot04_0f(obj);
}

void advance_or_reset_states_on_frame_param_slot04_0f(Object *obj) {
    if (data_80190458.p->frame->frame_param == 0) {
        obj->state_0 = obj->state_0 + 1;
    } else {
        obj->state_0 = 1;
        obj->active_count = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->health = 0xff;
    }
    advance_sequence_checked(obj);
    func_801b5b4c_slot04_0f(obj);
}

void func_801b5aac_slot04_0f(Object *obj) {
    advance_or_reset_states_on_frame_param_slot04_0f(obj);
}

void func_801b5acc_slot04_0f(Object *obj) {
    advance_or_reset_states_on_frame_param_slot04_0f(obj);
}

void func_801b5aec_slot04_0f(Object *obj) {
    advance_or_reset_states_on_frame_param_slot04_0f(obj);
}

void release_object_record_slot04_0f(Object *obj) {
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void release_object_record_2_slot04_0f(Object *obj) {
    reset_and_push_block_object_type_8((Slab172 *)obj);
}
