/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_80190464;
extern s16 data_8019046c;

void func_801280f0(void) {
    while (game_state.field_f0 != 0) {
        func_801192bc(1);
    }
    game_state.field_f0 = 0x1f;
    data_80190464 = 1;
    data_8019046c = 0;
    func_80119144(2, 4);
    while (game_state.field_ee != 0) {
        func_801192bc(1);
    }
}

void func_8012818c(void) {
    while (game_state.field_f0 != 0) {
        func_801192bc(1);
    }
    game_state.field_f0 = 0x1f;
    data_80190464 = 0xc01;
    data_8019046c = -1;
    func_80119144(2, 4);
    while (game_state.field_ee != 0) {
        func_801192bc(1);
    }
}

void func_8012822c(void) {
    if (game_state.field_ee == 0) {
        game_state.field_f0 = 0x1f;
        data_80190464 = 0x1e01;
        data_8019046c = 0x1f00;
        func_80119144(2, 4);
    }
}

void func_80128280(void) {
    if (game_state.field_ee == 0) {
        game_state.field_f0 = 0x1f;
        data_80190464 = 0x2001;
        data_8019046c = 0x1f00;
        func_80119144(2, 4);
    }
}

void func_801282d4(void) {
    while (game_state.field_f0 != 0) {
        func_801192bc(1);
    }
    game_state.field_f0 = 0x1f;
    data_80190464 = 0x1801;
    data_8019046c = 0;
    func_80119144(2, 4);
    while (game_state.field_ee != 0) {
        func_801192bc(1);
    }
}

void func_80128370(void) {
    while (game_state.field_f0 != 0) {
        func_801192bc(1);
    }
    game_state.field_f0 = 0x1f;
    data_80190464 = 0x1a01;
    data_8019046c = -1;
    func_80119144(2, 4);
    while (game_state.field_ee != 0) {
        func_801192bc(1);
    }
}

void func_80128410(void) {
    if (game_state.field_ee == 0) {
        game_state.field_f0 = 0x1f;
        data_80190464 = 0x2a01;
        data_8019046c = 0x1f00;
        func_80119144(2, 4);
    }
}
