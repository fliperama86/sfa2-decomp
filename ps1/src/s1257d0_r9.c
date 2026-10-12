/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80126620(GameState *state, Object *object) {
    if ((*(u32 *)&state->field_44 & 0xff00ff) != 0 || state->field_09 != 0)
        func_801269e0(object);
    else if (object->phase_index == 0)
        func_80126698(state, object);
    else
        func_801266c8(state, object);
}

void func_80126698(GameState *state, Object *object) {
    object->phase_index++;
    func_80135c80(object, 0xc);
}

void func_801266c8(GameState *state, Object *object) {
    func_80135c80(object, 0xc);
    if (func_80126e98(state, object)) {
        object->step_index = 2;
        object->phase_index = 0;
        object->score = 0;
        object->score_2 = 0;
        object->db_flag = 0;
        object->field_ef = 0;
        object->hit_count = 0;
        object->counter_ed = 0;
        object->repeat_count = 0;
        object->cost_index = 0;
        object->saved_side = object->side;
        func_80126778(state, object);
    }
}

void func_80126758(GameState *state, Object *object) {
    func_80126778(state, object);
}

void func_80126778(GameState *state, Object *object) {
    if (state->state_46_flag)
        func_80126d68(state, object);
    else if (object->phase_index == 0)
        func_801267d8(state, object);
    else
        func_80126930(state, object);
}

void func_801267d8(GameState *state, Object *object) {
    if (state->field_09 != 0 || state->field_04 != 0)
        func_801269e0(object);
    else if (state->active_flag != 0)
        func_80126898(state, object);
    else if (state->alt_flag != 0)
        func_801268fc(state, object);
    else {
        object->phase_index++;
        state->field_04 = 0xff;
        state->pause_flag = 0xff;
        state->fade_busy = 0;
        state->wait_flag = 0;
        state->field_4f = 1;
        state->field_6d = 0;
        state->field_74 = 0;
        func_80120408();
    }
}

void func_80126898(GameState *state, Object *object) {
    if (state->side_bits != 3) func_801388a8(state, object);
    state->active_flag = 0xff;
    if (state->field_80 == 0) state->state_2c_flag = 0;
    func_801268fc(state, object);
}

void func_801268fc(GameState *state, Object *object) {
    func_80126a00(state, object);
    func_80135c80(object, 0);
}

void func_80126930(GameState *state, Object *object) {
    if (state->field_04 != 0)
        func_801269e0(object);
    else {
        func_80126a00(state, object);
        data_801fc248[0] = 1;
        state->step_counter = 0;
        data_801fc248[1] = 0;
        state->step_delay_timer = 0;
        data_801fc248[2] = 0;
        state->countdown_2 = 0;
        data_801fc248[3] = 0;
        state->timer_or_bits = 0;
        data_801fc248[4] = 0;
        state->countdown_timer_2 = 0;
        data_801fc248[5] = 0;
        state->field_cc = 0;
        func_80135c80(object, 0);
    }
}

void func_801269e0(Object *object) {
    func_80135c80(object, 0);
}
