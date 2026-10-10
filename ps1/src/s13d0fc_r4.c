/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013dce0(Object *object, u8 index, u8 unused) {
    Quad order = data_8016d9a4;
    int step = object->slots[index].field_01;

    game_state.field_35c = step;
    data_80188f4c = (step + 1) & 3;
    data_80188f50 = (step - 1) & 3;
    data_80188f4c = order.v[data_80188f4c];
    data_80188f50 = order.v[data_80188f50];
}

void func_8013dd8c(Object *object, u8 index) {
    Quad order = data_8016d9a4;
    int step = object->slots[index].field_01;

    game_state.field_35c = step;
    if (object->slots[index].field_03 & 0x80) {
        game_state.field_35c = step - 1;
    } else {
        game_state.field_35c = step + 1;
    }
    data_80188f4c = order.v[game_state.field_35c &= 3];
}

int func_8013de2c(Object *object, u8 index, u8 arg) {
    u8 r;
    table_8017abd8[object->slots[index].field_00](object, index, arg);
    r = data_80188f44;
    return r;
}
