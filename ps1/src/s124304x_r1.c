/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Declared here because the published headers lack them (inferred types). */
extern TextBuf data_8016e92c;
extern TextBuf data_8016e95c;
extern TextBuf data_8016e98c;
extern Triple data_80185fac[];
extern u8 data_80185fbc;
extern u8 data_80185fc0;
extern u8 data_80185fc4;
extern u8 data_8016e801;
extern u8 data_8016e802;
extern u8 data_8016e803;
extern HudState *data_8018f5a0;
void func_801246cc(Object *a, Triple *b);
void func_801246a4(ObjectView *a, Object *b, Object *c);

/* What it does (inferred, not original names): per-frame update of the
   demo/replay recorder. It first runs func_801519b4 on four text buffers
   and func_801246cc on each of the two players with their pad record
   (data_80185fac, 3 bytes each). If game_state.side_bits is not 0 and one
   of the two pad records has a non-zero third byte (c), it acts on the
   mode byte data_8016e800:
     - 4, 5 or 6: clears both c bytes and does nothing else;
     - otherwise: increments pass_index of the HUD block data_8018f5a0,
       clears the input block data_801a6984 (field_00, field_01, index, last,
       flag), clears round_count, field_19f, hit_count_2, field_1a1 and field_1a2
       of both players, calls func_8013788c on each player, clears
       game_state.pending_flag and field_6d, calls set_selected_flag_by_index with
       data_8016e801 and func_80124ae8 with data_8016e802, and copies
       data_8016e803 to the input block's selection. Then by mode:
         1: calls func_801246a4(game_state, left, right); sets the input
            block's field_01 to 1; data_801ae028 = 0x88; saves the three
            bytes data_8016e801..803 in data_80185fbc, fc0, fc4; calls
            set_selected_flag_by_index and func_80124ae8 again with the first two;
            game_state.round_time = 0x63; selection = data_8016e803 again; and
            zeroes the 4096 halfwords of the input block's buffer;
         2: calls func_801246a4(game_state, left, right); sets the input
            block's field_00 to 1; calls set_selected_flag_by_index with data_80185fbc
            and func_80124ae8 with data_80185fc0; game_state.round_time =
            0x63; selection = data_80185fc4;
         3: clears both c bytes; sets the HUD block's phase_index to 1 and
            step_index, substep_index, step_index_2 and pass_index to 0; game_state
            alt_flag = 1, mode = 0, side_mask = side_bits;
         any other value: nothing more. */
void func_80124304(void) {
    int i;
    u16 blank;
    InputBlock *p;
    GameState *g;
    Triple *t;
    Object *l;
    Object *r;

    func_801519b4(&data_8016e8fc);
    func_801519b4(&data_8016e92c);
    func_801519b4(&data_8016e95c);
    func_801519b4(&data_8016e98c);
    l = &player_left;
    func_801246cc(l, t = data_80185fac);
    r = l + 1;
    func_801246cc(r, ++t);
    p = (InputBlock *)data_801a6984;
    g = &game_state;
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With 0 written in the loop that stores it, this function differs from the original in 14 instruction slots. */
    blank = 0;
    /* Stand-in, not recovered source: the term that is ORed to game_state.side_bits in the test below, which is 0 for every value of t, reproduces the original's choice of registers. Without it this function differs from the original in 5 instruction slots. */
    /* The original steps the pad record pointer in its own register and then copies it to the second argument register. With no later use of the pointer the compiler makes one instruction of the two; the term is such a use and leaves no instruction of its own. */
    /* The pointer goes to an unsigned integer of 64 bits first and from there to 32 bits: the first conversion holds every pointer of the 32-bit and 64-bit compilers this project builds with, and the second is the defined narrowing of an unsigned integer. A direct conversion of a wider pointer to 32 bits would not be defined. */
    if ((game_state.side_bits | (((u32)(unsigned long long)t << 24) & 0x100)) != 0 && (data_80185fac[0].c != 0 || data_80185fac[1].c != 0)) {
        if (data_8016e800 == 4 || data_8016e800 == 5 || data_8016e800 == 6) {
            data_80185fac[0].c = 0;
            data_80185fac[1].c = 0;
        } else {
            data_8018f5a0->pass_index++;
            p->index = 0;
            p->last = 0;
            p->flag = 0;
            p->field_00 = 0;
            p->field_01 = 0;
            l->round_count = 0;
            l->field_19f = 0;
            l->hit_count_2 = 0;
            l->field_1a1 = 0;
            l->field_1a2 = 0;
            r->round_count = 0;
            r->field_19f = 0;
            r->hit_count_2 = 0;
            r->field_1a1 = 0;
            r->field_1a2 = 0;
            func_8013788c(l);
            func_8013788c(r);
            g->pending_flag = 0;
            g->field_6d = 0;
            set_selected_flag_by_index(data_8016e801);
            func_80124ae8(data_8016e802);
            p->selection = data_8016e803;
            if (data_8016e800 == 1) {
                func_801246a4((ObjectView *)g, l, r);
                p->field_01 = 1;
                data_801ae028 = 0x88;
                data_80185fbc = data_8016e801;
                data_80185fc0 = data_8016e802;
                data_80185fc4 = data_8016e803;
                set_selected_flag_by_index(data_8016e801);
                func_80124ae8(data_8016e802);
                g->round_time = 0x63;
                p->selection = data_8016e803;
                for (i = 0xfff; i >= 0; i--) {
                    p->buf[i] = blank;
                }
            } else if (data_8016e800 == 2) {
                func_801246a4((ObjectView *)g, l, r);
                p->field_00 = 1;
                set_selected_flag_by_index(data_80185fbc);
                func_80124ae8(data_80185fc0);
                g->round_time = 0x63;
                p->selection = data_80185fc4;
            } else if (data_8016e800 == 3) {
                data_80185fac[0].c = 0;
                data_80185fac[1].c = 0;
                data_8018f5a0->phase_index = 1;
                data_8018f5a0->step_index = 0;
                data_8018f5a0->substep_index = 0;
                data_8018f5a0->step_index_2 = 0;
                data_8018f5a0->pass_index = 0;
                g->alt_flag = 1;
                g->mode = 0;
                g->side_mask = g->side_bits;
            }
        }
    }
}
