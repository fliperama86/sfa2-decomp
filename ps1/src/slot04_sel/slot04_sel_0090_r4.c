/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Slot04SelRec data_801b9cf0_slot04_sel[];
extern TextBuf *data_801b7570_slot04_sel[];
extern TextBuf *data_801b7618_slot04_sel[];
extern u8 *data_801b7654_slot04_sel;
extern u8 data_801b7348_slot04_sel[];
extern u8 data_8016e694[];
extern u8 data_8016e696;

void func_801b038c_slot04_sel(void) {
    Object *l = &player_left;
    Object *r = l + 1;
    Slot04SelRec *b = &data_801b9cf0_slot04_sel[1];
    TextBuf *p;
    TextBuf *q;

    if (game_state.field_07 & 1) {
        if (data_801b9cf0_slot04_sel[0].field_03 == 0x12) {
            data_801b9cf0_slot04_sel[0].field_03 = 4;
        }
        if (data_801b9cf0_slot04_sel[0].field_03 == 0x13) {
            data_801b9cf0_slot04_sel[0].field_03 = 0x11;
        }
        if (data_801b9cf0_slot04_sel[0].field_03 == 0x14) {
            data_801b9cf0_slot04_sel[0].field_03 = 2;
        }
    }
    if (game_state.field_07 & 2) {
        if (b->field_03 == 0x12) {
            b->field_03 = 4;
        }
        if (b->field_03 == 0x13) {
            b->field_03 = 0x11;
        }
        if (b->field_03 == 0x14) {
            b->field_03 = 2;
        }
    }
    data_8018f5a0->field_50 += 1;
    p = data_801b7570_slot04_sel[0];
    q = data_801b7618_slot04_sel[0];
    p->buf[4] = 0x1b;
    if (data_8016e694[l->side]) {
        q->buf[data_801b7348_slot04_sel[0]] = 0x1b;
        q->buf[data_801b7348_slot04_sel[1]] = 0x1a;
    } else {
        q->buf[data_801b7348_slot04_sel[0]] = 0x1a;
        q->buf[data_801b7348_slot04_sel[1]] = 0x1b;
    }
    p = data_801b7570_slot04_sel[1];
    q = data_801b7618_slot04_sel[1];
    p->buf[4] = 0x1b;
    if (data_8016e694[r->side]) {
        q->buf[data_801b7348_slot04_sel[0]] = 0x1b;
        q->buf[data_801b7348_slot04_sel[1]] = 0x1a;
    } else {
        q->buf[data_801b7348_slot04_sel[0]] = 0x1a;
        q->buf[data_801b7348_slot04_sel[1]] = 0x1b;
    }
    if (data_8016e696) {
        data_801b7654_slot04_sel[data_801b7348_slot04_sel[4]] = 0x1b;
        data_801b7654_slot04_sel[data_801b7348_slot04_sel[5]] = 0x1a;
    } else {
        data_801b7654_slot04_sel[data_801b7348_slot04_sel[4]] = 0x1a;
        data_801b7654_slot04_sel[data_801b7348_slot04_sel[5]] = 0x1b;
    }
}
