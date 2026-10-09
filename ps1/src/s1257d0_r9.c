/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801388a8(GameState *state, Object *object);

void func_80126620(GameState *state, Object *object) {
    if ((*(u32 *)&state->field_44 & 0xff00ff) != 0 || state->field_09 != 0)
        func_801269e0(object);
    else if (object->field_aa == 0)
        func_80126698(state, object);
    else
        func_801266c8(state, object);
}

void func_80126698(GameState *state, Object *object) {
    object->field_aa++;
    func_80135c80(object, 0xc);
}

void func_801266c8(GameState *state, Object *object) {
    func_80135c80(object, 0xc);
    if (func_80126e98(state, object)) {
        object->field_a9 = 2;
        object->field_aa = 0;
        object->field_b8 = 0;
        object->field_e4 = 0;
        object->field_db = 0;
        object->field_ef = 0;
        object->field_ec = 0;
        object->field_ed = 0;
        object->field_ee = 0;
        object->field_c1 = 0;
        object->field_118 = object->side;
        func_80126778(state, object);
    }
}

void func_80126758(GameState *state, Object *object) {
    func_80126778(state, object);
}

void func_80126778(GameState *state, Object *object) {
    if (state->field_46)
        func_80126d68(state, object);
    else if (object->field_aa == 0)
        func_801267d8(state, object);
    else
        func_80126930(state, object);
}

void func_801267d8(GameState *state, Object *object) {
    if (state->field_09 != 0 || state->field_04 != 0)
        func_801269e0(object);
    else if (state->field_27 != 0)
        func_80126898(state, object);
    else if (state->field_1a != 0)
        func_801268fc(state, object);
    else {
        object->field_aa++;
        state->field_04 = 0xff;
        state->field_05 = 0xff;
        state->field_ee = 0;
        state->field_f0 = 0;
        state->field_4f = 1;
        state->field_6d = 0;
        state->field_74 = 0;
        func_80120408();
    }
}

void func_80126898(GameState *state, Object *object) {
    if (state->field_17 != 3) func_801388a8(state, object);
    state->field_27 = 0xff;
    if (state->field_80 == 0) state->field_2c = 0;
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
        state->field_c2 = 0;
        data_801fc248[1] = 0;
        state->field_c4 = 0;
        data_801fc248[2] = 0;
        state->field_c6 = 0;
        data_801fc248[3] = 0;
        state->field_c8 = 0;
        data_801fc248[4] = 0;
        state->field_ca = 0;
        data_801fc248[5] = 0;
        state->field_cc = 0;
        func_80135c80(object, 0);
    }
}

void func_801269e0(Object *object) {
    func_80135c80(object, 0);
}
