/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
Object *func_80125060(Object *object);

void func_801211dc(GameState *state) {
    if (data_8018f5a0->field_52 == 0) {
        Object *object;
        data_8018f5a0->field_52++;
        object = func_80125060((Object *)state);
        state->field_154 = object;
        func_8014eef8(object->kind, object->side);
    } else {
        func_80010058(state);
    }
}

void func_8012124c(GameState *unused) {
    func_80010b2c();
}

void func_8012126c(GameState *state) {
    if (data_8018f5a0->field_4c == 0) {
        data_8018f5a0->field_4c++;
        func_8014efa8(state->field_70 & 0x7f);
    } else {
        func_80013834();
    }
}

void func_801212c8(GameState *unused) {
    func_8001365c();
}

void func_801212e8(GameState *unused) {
    func_800136b0();
}

void func_80121308(GameState *state) {
    if (data_8018f5a0->field_52 == 0) {
        data_8018f5a0->field_52++;
        game_state.field_154 = state->field_78;
        func_8014eef8(state->field_70, state->field_227);
    } else {
        data_8018f5a0->field_4a = 4;
    }
}

void func_80121378(GameState *state) {
    table_8016e760[data_8018f5a0->field_4c](state);
}
