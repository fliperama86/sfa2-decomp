/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138838(GameState *state, int delta) {
    data_80188ec4 += delta;
    func_8013886c(state);
}

void func_8013886c(GameState *state) {
    if ((s16)data_80188ec4 < 0) {
        data_80188ec4 = 0;
    } else if ((s16)data_80188ec4 >= 0x100) {
        data_80188ec4 = 0xff;
    }
}

void func_801388a8(GameState *state, Object *object) {
    int cost;

    if (state->field_0a == 0) {
        func_8013839c(state);
        data_80188ecc = 1;
    }
    cost = 0x48;
    if (object->field_c1 < 0x10) cost = table_80176d70[object->field_c1];
    data_80188ec4 = data_80188ec4 - cost;
    func_80138838(state, table_80176d80[func_80151184() & 0x1f] - 0x18);
    func_8013886c(state);
}

void func_80138964(GameState *state) {
    short a;
    Object *other;

    if (state->mode == 3 && state->field_2f == 0) return;
    if (state->field_49 >= 0x50) {
        a = 0x10;
    } else if (state->field_49 >= 0x3c) {
        a = 6;
    } else if (state->field_49 >= 0x28) {
        a = 4;
    } else if (state->field_49 >= 0x14) {
        a = 6;
    } else {
        a = 0xa;
    }
    func_80138838(state, a);
    other = state->field_78;
    if (other != 0 && other->field_cd == 0) {
        if ((s16)other->field_5c >= 0x70) {
            a = 0x10;
        } else if ((s16)other->field_5c >= 0x50) {
            a = 0xa;
        } else if ((s16)other->field_5c >= 0x30) {
            a = 8;
        } else {
            a = -((s16)other->field_5c < 0x10) & 0xfff8;
        }
        func_80138838(state, a);
    }
    if (state->field_4d != 0) {
        a = 2;
        other = state->field_78;
        if (other != 0 && other->field_cd != 0) a = -8;
        func_80138838(state, a);
    }
    func_80138628(state);
    func_8013886c(state);
    func_8013860c(state);
}

void func_80138ac8(GameState *state, Object *object) {
    func_80138ae8(state, object);
}

void func_80138ae8(GameState *state, Object *object) {
    if ((state->mode != 3 || state->field_2f != 0) && object->field_cd == 0)
        func_80138838(state, 1);
}

void func_80138b38(GameState *state, Object *object) {
    object->field_225 = 1;
    if ((state->mode != 3 || state->field_2f != 0) && object->field_cd == 0)
        func_80138838(state, object->field_12a + 2);
}

void func_80138b94(GameState *state, Object *object) {
    if ((state->mode != 3 || state->field_2f != 0) && object->field_cd == 0)
        func_80138838(state, 3);
}

void func_80138be4(GameState *state, Object *object) {
    if ((state->mode != 3 || state->field_2f != 0) && object->field_cd == 0 &&
        object->field_6a >= 2) {
        Object *other = object->other;
        if (*(u16 *)&other->field_04 == 2 && (u8)(other->field_06 - 5) < 2)
            func_80138838(state, 1);
    }
}
