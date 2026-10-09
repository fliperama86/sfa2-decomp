/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_801b8320_slot04_sel[];
extern TextBuf *data_801b8540_slot04_sel[];
extern TextBuf *data_801b85e8_slot04_sel[];
extern TextBuf data_801b8618_slot04_sel;
extern u8 data_801b9d3b_slot04_sel;
extern u8 data_801b9d4d_slot04_sel[];

void func_801b2580_slot04_sel(void) {
    Object *left = &player_left;
    Object *right = left + 1;
    TextBuf *t;
    u8 *q = data_801b9d4d_slot04_sel;
    if (game_state.field_07 & 1) {
        if (data_801b9d3b_slot04_sel == 0x12) {
            data_801b9d3b_slot04_sel = 4;
        }
        if (data_801b9d3b_slot04_sel == 0x13) {
            data_801b9d3b_slot04_sel = 0x11;
        }
        if (data_801b9d3b_slot04_sel == 0x14) {
            data_801b9d3b_slot04_sel = 2;
        }
    }
    if (game_state.field_07 & 2) {
        if (q[3] == 0x12) {
            q[3] = 4;
        }
        if (q[3] == 0x13) {
            q[3] = 0x11;
        }
        if (q[3] == 0x14) {
            q[3] = 2;
        }
    }
    data_8018f5a0->field_50++;
    t = data_801b85e8_slot04_sel[0];
    data_801b8540_slot04_sel[0]->buf[4] = 0x1b;
    if (data_8016e69a[left->side] != 0) {
        t->buf[data_801b8320_slot04_sel[0]] = 0x1b;
        t->buf[data_801b8320_slot04_sel[1]] = 0x1a;
    } else {
        t->buf[data_801b8320_slot04_sel[0]] = 0x1a;
        t->buf[data_801b8320_slot04_sel[1]] = 0x1b;
    }
    t = data_801b85e8_slot04_sel[1];
    data_801b8540_slot04_sel[1]->buf[4] = 0x1b;
    if (data_8016e69a[right->side] != 0) {
        t->buf[data_801b8320_slot04_sel[0]] = 0x1b;
        t->buf[data_801b8320_slot04_sel[1]] = 0x1a;
    } else {
        t->buf[data_801b8320_slot04_sel[0]] = 0x1a;
        t->buf[data_801b8320_slot04_sel[1]] = 0x1b;
    }
    if (data_8016e69a[2] != 0) {
        data_801b8618_slot04_sel.buf[data_801b8320_slot04_sel[4]] = 0x1b;
        data_801b8618_slot04_sel.buf[data_801b8320_slot04_sel[5]] = 0x1a;
    } else {
        data_801b8618_slot04_sel.buf[data_801b8320_slot04_sel[4]] = 0x1a;
        data_801b8618_slot04_sel.buf[data_801b8320_slot04_sel[5]] = 0x1b;
    }
}
