/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80136c8c(void) {
    table_80172500[game_state.field_40]();
}

void func_80136ccc(void) {
    if (data_80190568 != 0) {
        table_801725a0[game_state.field_40]();
    }
}

void func_80136d1c(Tx *tx) {
    func_80158a2c(tx, 0, 0, 0, 0);
    tx->field_0f = 3;
    tx->field_13 = 0x7c;
    func_8015c23c(tx, &tx->field_0c);
}

void func_80136d70(Tx *tx) {
    func_80158a2c(tx, 0, 0, 0, 0);
    tx->field_0f = 4;
    tx->field_13 = 0x64;
    func_8015c23c(tx, &tx->field_0c);
}

void func_80136dc4(void) {
    func_80136dfc();
    func_80137004();
    func_80137070();
    func_801370dc();
}

void func_80136dfc(void) {
    func_80136f90();
    func_80137148();
    func_801371b4();
    func_80136e68();
    func_80137364(4, 0, 0xb0, 0x1e0, 0xe);
    func_80137220(0, 6);
    func_80137220(4, 7);
}

void func_80136e68(void) {
    func_80136e90();
    func_80136ed0();
}

void func_80136e90(void) {
    func_80136f10(&player_left, 0x10, 0x100, player_left.field_d4 * 5 + 0x1e0);
}

void func_80136ed0(void) {
    func_80136f10(&player_right, 0xb, 0x120, player_right.field_d4 * 5 + 0x1e0);
}

void func_80136f10(Object *object, short index, int w, int h) {
    Rect rect;

    object->field_f4 = index;
    rect.x = w;
    rect.y = h;
    rect.w = 0x10;
    rect.h = 5;
    func_80158028(&rect, table_801a27e4 + (index << 5));
    func_80158028(&rect, table_801a27e4 + 0x1400 + (index << 5));
    data_801aa4dc[0] = 0x1f;
}
