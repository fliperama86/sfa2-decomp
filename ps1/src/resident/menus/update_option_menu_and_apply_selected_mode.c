/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

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
extern HudState *screen_state;
void update_option_menu_selection(Object *a, Triple *b);
void reset_fields_and_clear_position_fracs(ObjectView *a, Object *b, Object *c);

/* What it does (inferred, not original names): per-frame update of the
   demo/replay recorder. It first runs add_item_to_table on four text buffers
   and update_option_menu_selection on each of the two players with their pad record
   (data_80185fac, 3 bytes each). If game_state.side_bits is not 0 and one
   of the two pad records has a non-zero third byte (c), it acts on the
   mode byte data_8016e800:
     - 4, 5 or 6: clears both c bytes and does nothing else;
     - otherwise: increments pass_index of the HUD block screen_state,
       clears the input block input_block (field_00, field_01, index, last,
       flag), clears round_count, field_19f, hit_count_2, field_1a1 and field_1a2
       of both players, calls call_side_func_then_queue_job on each player, clears
       game_state.pending_flag and field_6d, calls set_selected_flag_by_index with
       data_8016e801 and set_table_index_3_from_table with data_8016e802, and copies
       data_8016e803 to the input block's selection. Then by mode:
         1: calls reset_fields_and_clear_position_fracs(game_state, left, right); sets the input
            block's field_01 to 1; data_801ae028 = 0x88; saves the three
            bytes data_8016e801..803 in data_80185fbc, fc0, fc4; calls
            set_selected_flag_by_index and set_table_index_3_from_table again with the first two;
            game_state.round_time = 0x63; selection = data_8016e803 again; and
            zeroes the 4096 halfwords of the input block's buffer;
         2: calls reset_fields_and_clear_position_fracs(game_state, left, right); sets the input
            block's field_00 to 1; calls set_selected_flag_by_index with data_80185fbc
            and set_table_index_3_from_table with data_80185fc0; game_state.round_time =
            0x63; selection = data_80185fc4;
         3: clears both c bytes; sets the HUD block's phase_index to 1 and
            step_index, substep_index, step_index_2 and pass_index to 0; game_state
            alt_flag = 1, mode = 0, side_mask = side_bits;
         any other value: nothing more. */
void update_option_menu_and_apply_selected_mode(void) {
    int i;
    u16 blank;
    InputBlock *p;
    GameState *g;
    Triple *t;
    Object *l;
    Object *r;

    add_item_to_table(&data_8016e8fc);
    add_item_to_table(&data_8016e92c);
    add_item_to_table(&data_8016e95c);
    add_item_to_table(&data_8016e98c);
    l = &player_left;
    update_option_menu_selection(l, t = data_80185fac);
    r = l + 1;
    update_option_menu_selection(r, ++t);
    p = (InputBlock *)input_block;
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
            screen_state->pass_index++;
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
            call_side_func_then_queue_job(l);
            call_side_func_then_queue_job(r);
            g->pending_flag = 0;
            g->field_6d = 0;
            set_selected_flag_by_index(data_8016e801);
            set_table_index_3_from_table(data_8016e802);
            p->selection = data_8016e803;
            if (data_8016e800 == 1) {
                reset_fields_and_clear_position_fracs((ObjectView *)g, l, r);
                p->field_01 = 1;
                data_801ae028 = 0x88;
                data_80185fbc = data_8016e801;
                data_80185fc0 = data_8016e802;
                data_80185fc4 = data_8016e803;
                set_selected_flag_by_index(data_8016e801);
                set_table_index_3_from_table(data_8016e802);
                g->round_time = 0x63;
                p->selection = data_8016e803;
                for (i = 0xfff; i >= 0; i--) {
                    p->buf[i] = blank;
                }
            } else if (data_8016e800 == 2) {
                reset_fields_and_clear_position_fracs((ObjectView *)g, l, r);
                p->field_00 = 1;
                set_selected_flag_by_index(data_80185fbc);
                set_table_index_3_from_table(data_80185fc0);
                g->round_time = 0x63;
                p->selection = data_80185fc4;
            } else if (data_8016e800 == 3) {
                data_80185fac[0].c = 0;
                data_80185fac[1].c = 0;
                screen_state->phase_index = 1;
                screen_state->step_index = 0;
                screen_state->substep_index = 0;
                screen_state->step_index_2 = 0;
                screen_state->pass_index = 0;
                g->alt_flag = 1;
                g->mode = 0;
                g->side_mask = g->side_bits;
            }
        }
    }
}
