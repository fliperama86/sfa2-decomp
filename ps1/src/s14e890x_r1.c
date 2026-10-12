/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Inferred declarations; not original ones. */
extern Rect data_80189470;
extern u32 table_8017d96c[];
extern u16 table_8017d9bc[];
void clear_four_arrays_and_call_kind_handlers(void);
void func_8015808c(Rect *rect, int a, int b);
void func_801451ac(int a);

/*
 * What it does (inferred, not an original name): sets up the texture memory
 * of a screen. It sets a flag byte, calls a fixed sequence of setup
 * routines, then uploads image data in 4 by 16 pixel strips to video memory:
 * first as many 128-byte strips as the current table entry says (the
 * entry is a byte count, selected by game_state.kind_index), placed in a grid
 * of 16 rows by 16 columns of strips per page, starting at an x offset taken
 * from a second table; then three 16 by 32 clears, one 16 by 7 upload, and
 * a fixed 949 strips from another buffer in the same grid layout (16 by 16
 * strips a page, 64 pixels of x a page) but starting at page x 0x180, with y
 * from 0 instead of 0x100 and without the second table's offset. It ends
 * with one more routine call.
 *
 */
void load_kind_images_to_vram(int arg) {
    int row;
    int col;
    int page;
    int page2;
    unsigned i;
    GameState *g;
    int ct;
    int rt;
    int k;
    int k2;
    Rect *q;

    data_8019032d = 1;
    set_voice_thread_entry_and_open(2, 7);
    request_sound_command_and_switch_thread(1);
    close_vab_and_sep(2);
    close_vab_and_sep(3);
    start_cd_track_wait_with_thread_switch(3, 1);
    start_cd_track_wait_with_thread_switch(2, arg);
    /* The local g only feeds the stand-in term below. */
    g = &game_state;
    clear_four_arrays_and_call_kind_handlers();
    row = 0;
    col = 0;
    page = 0;
    data_80189470.w = 4;
    data_80189470.h = 0x10;
    for (i = 0; i < (table_8017d96c[game_state.kind_index] >> 7); i++) {
        data_80189470.x = table_8017d9bc[game_state.kind_index] + ((row & 0xff) << 2) + (page << 6);
        data_80189470.y = ((col & 0xff) << 4) + 0x100;
        /* Stand-in, not recovered source: the term that is ORed to the constant 0x10 below, which is 0 for every value of g, reproduces the original's choice of registers. Without it this function differs from the original in 14 instruction slots. */
        k = 0x10 | (int)(((u32)(unsigned long long)g << 24) & 0x100);
        func_80157fc4(&data_80189470, (u8 *)0x80030000 + (i << 7));
        col++;
        /* Stand-in, not recovered source: the term that is ORed to the masked value of col below, which is 0 for every value of col, reproduces the original's choice of registers. Without it this function differs from the original in 1 instruction slots. */
        ct = (col & 0xff) | (int)(((u32)col << 24) & 0x100);
        if (ct == k) {
            row++;
        }
        /* Stand-in, not recovered source: the term that is ORed to the masked value of row below, which is 0 for every value of row, reproduces the original's choice of registers. Without it this function differs from the original in 15 instruction slots. */
        rt = (row & 0xff) | (int)(((u32)row << 24) & 0x100);
        if (rt == k) {
            page++;
        }
        row = rt & 0xf;
        col = ct & 0xf;
        /* Stand-in, not recovered source: the term that is ORed to page below, which is 0 for every value of page, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
        page |= (int)(((u32)page << 24) & 0x100);
    }
    data_80189470.x = 0;
    data_80189470.y = 0x1e0;
    data_80189470.w = 16;
    data_80189470.h = 32;
    func_8015808c(&data_80189470, 0xd0, 0x1e0);
    data_80189470.x = 16;
    data_80189470.y = 0x1e0;
    data_80189470.w = 16;
    data_80189470.h = 32;
    func_8015808c(&data_80189470, 0xe0, 0x1e0);
    data_80189470.x = 32;
    data_80189470.y = 0x1e0;
    data_80189470.w = 16;
    data_80189470.h = 32;
    func_8015808c(&data_80189470, 0xf0, 0x1e0);
    data_80189470.x = 0xc0;
    data_80189470.y = 0x1e0;
    data_80189470.w = 16;
    data_80189470.h = 7;
    func_80157fc4(&data_80189470, (u8 *)0x80050000);
    row = 0;
    col = 0;
    data_80189470.w = 4;
    data_80189470.h = 16;
    for (i = 0, q = &data_80189470, k2 = 0x10, page2 = 0x180; (int)i < 0x3b5; i++) {
        q->x = ((row & 0xff) << 2) + page2;
        q->y = (col & 0xff) << 4;
        func_80157fc4(q, (u8 *)0x80070000 + (i << 7));
        col++;
        /* Stand-in, not recovered source: the term that is ORed to the masked value of col below, which is 0 for every value of col, reproduces the original's choice of registers. Without it this function differs from the original in 1 instruction slots. */
        ct = (col & 0xff) | (int)(((u32)col << 24) & 0x100);
        if (ct == k2) {
            row++;
        }
        /* Stand-in, not recovered source: the term that is ORed to the masked value of row below, which is 0 for every value of row, reproduces the original's choice of registers. Without it this function differs from the original in 15 instruction slots. */
        rt = (row & 0xff) | (int)(((u32)row << 24) & 0x100);
        if (rt == k2) {
            page2 += 0x40;
        }
        row = rt & 0xf;
        col = ct & 0xf;
    }
    func_801451ac(0);
}
