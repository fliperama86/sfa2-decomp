/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has the same size but differs from the original's bytes in
 * register choice and instruction order. The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the original; the build does not use this
 * file. The differential test next to it (func_8014e890.py, run by
 * difftest.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): sets up the texture memory
 * of a screen. It sets a flag byte, calls a fixed sequence of setup
 * routines, then uploads image data in 4 by 16 pixel strips to video memory:
 * first as many 128-byte strips as the current table entry says (the
 * entry is a byte count, selected by game_state.field_40), placed in a grid
 * of 16 rows by 16 columns of strips per page, starting at an x offset taken
 * from a second table; then three 16 by 32 clears, one 16 by 7 upload, and
 * a fixed 949 strips from another buffer in the same grid layout starting at
 * page x 0x180. It ends with one more routine call.
 *
 * Contract (the roles of the names are inferred):
 *   Argument: a0 = a value passed on to the second call of func_8014f3b8.
 *   No return value.
 *   Reads: game_state.field_40 (index, 0 to 19), table_8017d96c[field_40]
 *     (a byte count; the number of strips of the first loop is that count
 *     divided by 128, as an unsigned value), table_8017d9bc[field_40].
 *   The buffers data_80030000, data_80050000 and data_80070000 are fixed
 *     addresses in the original; the names are the project's.
 *   Watched at every call (copied into the log): the word holding
 *     data_8019032d and the rectangle data_80189470 (two words), so the order
 *     of the function's stores against its calls is compared.
 *   Writes: data_8019032d (set to 1) and the shared rectangle
 *     data_80189470 (x, y, w, h), in every step.
 *   Callees, all replaced by recorders returning 0 (the log shows each call
 *     in order with its arguments): func_80119144 (2 arguments),
 *     func_801192bc (1), func_801203b4 (1), func_8014f3b8 (2),
 *     func_80136bc0 (0), func_80157fc4 (2), func_8015808c (3),
 *     func_801451ac (1). For func_80157fc4 and func_8015808c the
 *     recorder also copies the rectangle (8 bytes) that the first argument
 *     points at, at every call, so the values the function wrote for that
 *     call are compared. func_80157fc4 and func_8015808c are Sony library
 *     routines; the others are replaced because they would need contracts of
 *     their own.
 *   Excluded inputs: field_40 above 19 (it indexes past the tables, and the
 *     loop count would be whatever data follows); a count of strips above
 *     about 300, which only lengthens the run.
 *   Aliasing: nothing but the rectangle, the flag byte and the log is
 *     written; the tables, the rectangle and the log are distinct.
 *   Not reached by any input: none expected; the coverage line of the test
 *     states it.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; not original ones. */
extern Rect data_80189470;
extern u32 table_8017d96c[];
extern u16 table_8017d9bc[];
extern u8 data_80030000[];
extern u8 data_80050000[];
extern u8 data_80070000[];
void func_80136bc0(void);
void func_8015808c(Rect *rect, int a, int b);
void func_801451ac(u8 flag);

void func_8014e890(int arg) {
    int row;
    int col;
    int page;
    unsigned i;

    data_8019032d = 1;
    func_80119144(2, 7);
    func_801192bc(1);
    func_801203b4(2);
    func_801203b4(3);
    func_8014f3b8(3, 1);
    func_8014f3b8(2, arg);
    func_80136bc0();

    row = 0;
    col = 0;
    page = 0;
    data_80189470.w = 4;
    data_80189470.h = 16;
    for (i = 0; i < (table_8017d96c[game_state.field_40] >> 7); i++) {
        data_80189470.x = table_8017d9bc[game_state.field_40] + (row << 2) + (page << 6);
        data_80189470.y = (col << 4) + 0x100;
        col++;
        func_80157fc4(&data_80189470, data_80030000 + (i << 7));
        if (col == 16) {
            col = 0;
            row++;
        }
        if (row == 16) {
            row = 0;
            page++;
        }
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
    func_80157fc4(&data_80189470, data_80050000);

    row = 0;
    col = 0;
    page = 0x180;
    data_80189470.w = 4;
    data_80189470.h = 16;
    for (i = 0; i < 0x3b5; i++) {
        data_80189470.x = (row << 2) + page;
        data_80189470.y = col << 4;
        func_80157fc4(&data_80189470, data_80070000 + (i << 7));
        col++;
        if (col == 16) {
            col = 0;
            row++;
        }
        if (row == 16) {
            row = 0;
            page += 0x40;
        }
    }
    func_801451ac(0);
}
