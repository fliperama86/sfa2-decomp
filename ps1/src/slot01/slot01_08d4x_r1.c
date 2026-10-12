/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

typedef struct {
    u8 pad0[4];
    u16 x;
    u8 pad6[6];
    char *text;
} Slot01TextRec;

extern Object *data_8002cea8_slot01;
extern int data_8002cea4_slot01;
extern int data_8002cea0_slot01;
extern Strip1c *data_8002ce9c_slot01;
extern Slot01TextRec data_800150cc_slot01[];
void func_80010d0c_slot01(int value, char *buf);
void func_80010bf0_slot01(int value, char *buf);
/*
 * What it does (inferred, not an original name): draws the status panel of
 * the object that game_state.focus_object points at, unless its mode_flag_2 is not
 * 0. It picks a left position (0x10 or 0xe0 by team_id), sets the x of
 * text records in data_800150cc_slot01, formats two or three numbers into
 * their buffers and queues each record with func_801519b4. Last it fills
 * win_count + 1 strips and links them into the ordering table head of the
 * buffer data_801a27d0.
 */
void func_800108d4_slot01(void) {
    Object *o = game_state.focus_object;
    Slot01TextRec *q;
    data_8002cea8_slot01 = o;
    if (o->mode_flag_2 == 0) {
        if (o->team_id != 0) {
            data_8002cea4_slot01 = 0x10;
        } else {
            data_8002cea4_slot01 = 0xe0;
        }
        data_800150cc_slot01[0].x = data_8002cea4_slot01;
        data_800150cc_slot01[1].x = data_8002cea4_slot01 + 0x38;
        func_80010d0c_slot01(data_8002cea8_slot01->best_score, data_800150cc_slot01[1].text);
        func_801519b4(&data_800150cc_slot01[0]);
        func_801519b4(&data_800150cc_slot01[1]);
        data_800150cc_slot01[2].x = data_8002cea4_slot01;
        data_800150cc_slot01[3].x = data_8002cea4_slot01 + 0x48;
        func_80010bf0_slot01(game_state.select_flag != 0 ? data_8002cea8_slot01->win_streak : data_8002cea8_slot01->count, data_800150cc_slot01[3].text);
        func_801519b4(&data_800150cc_slot01[2]);
        func_801519b4(&data_800150cc_slot01[3]);
        data_800150cc_slot01[4].x = data_8002cea4_slot01;
        func_801519b4(&data_800150cc_slot01[4]);
        if (data_8002cea8_slot01->max_value_cc != 0) {
            data_800150cc_slot01[5].x = data_8002cea4_slot01 + 0x48;
            func_80010bf0_slot01(data_8002cea8_slot01->max_value_cc, data_800150cc_slot01[5].text);
            q = &data_800150cc_slot01[5];
        } else {
            if (data_8002cea8_slot01->team_id != 0) {
                data_800150cc_slot01[7].x = 0x78;
            } else {
                data_800150cc_slot01[7].x = 0x148;
            }
            q = &data_800150cc_slot01[7];
        }
        func_801519b4(q);
        data_800150cc_slot01[6].x = data_8002cea4_slot01;
        func_801519b4(&data_800150cc_slot01[6]);
        if (data_8002cea8_slot01->team_id != 0) {
            data_8002cea4_slot01 = 0x88;
            data_8002ce9c_slot01 = data_80190014[data_801a27d0];
        } else {
            data_8002cea4_slot01 = 0x158;
            data_8002ce9c_slot01 = strips[data_801a27d0];
        }
        data_8002cea0_slot01 = data_8002cea8_slot01->win_count;
        if (data_8002cea0_slot01 >= 0) {
            do {
                Strip1c *p = data_8002ce9c_slot01;
                PrimTag *ot;
                p->pos_x = data_8002cea4_slot01 - data_8002cea0_slot01 * 16;
                p->pos_y = 0x6e;
                /* The head of the ordering table of the buffer in use, 0xe8 bytes per buffer. Written through the symbol data_801fc050 the function differs in 12 instruction slots. */
                ot = (PrimTag *)(0x801fc050 + data_801a27d0 * 0xe8);
                ((PrimTag *)p)->addr = ot->addr;
                data_8002cea0_slot01--;
                data_8002ce9c_slot01 = p + 1;
                ot->addr = (u32)p;
            } while (data_8002cea0_slot01 >= 0);
        }
    }
}
