/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80136b34(Cam *cam);

void func_80136abc(void) {
    Cam *cam = (Cam *)data_801904d8;
    int a, b;
    if (data_801904d8[4] == 0) {
        func_80136b34(cam);
    } else if (data_801904d8[4] == 1) {
        func_80136b60(cam);
    }
    a = ((u16 *)&cam->field_64)[1];
    b = game_state.field_92;
    a = (a - b) & 0x3ff;
    game_state.field_da = a;
}
