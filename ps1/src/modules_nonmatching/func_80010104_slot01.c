/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code stores one global ahead of two zero stores (a scheduling
 * difference). The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py, with func_80010104_slot01.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): enters a mode of the
 * game. It saves game_state.field_40 in data_80055f3c_slot01 and replaces
 * it with data_801abf08, clears two globals, sets data_80190568 to 1, runs
 * three setup functions (the middle one only when data_801abf08 is not
 * 0x12), restores the field_2ac of the object game_state.field_78 points
 * at (the setup functions may change it), and loads a 16 x 2 block into
 * video memory. When data_80197f10 is at least 2 it also loads a 16 x 11
 * block and a 16 x 5 palette (80 halfwords, a table of this function),
 * allocates two objects with func_8011f1e0 (each is set up only if the
 * allocation succeeded; the second is also kept in data_8002ceb0_slot01
 * and data_80055f44_slot01), counts up the field_4e of data_8018f5a0, sets
 * a group of game_state and player fields, and calls four sound/setup
 * functions in sequence.
 *
 * Contract:
 *   No argument, no return value.
 *   Reads: game_state.field_78 (the object), field_40; the object's
 *     field_2ac and field_65; data_801abf08; data_80197f10 (byte);
 *     data_8018f5a0 (a pointer to a record of which field_4e is read and
 *     written, a halfword); the memory behind the two loaded blocks
 *     data_80015174_slot01 and data_80070d48 (the recorder copies it).
 *   Writes: data_80055f3c_slot01, data_801ac6a8 (byte), data_801aa5ea
 *     (halfword), game_state.field_40, data_80190568 (byte), the object's
 *     field_2ac; when data_80197f10 >= 2: for each allocated object field
 *     0, 2, 3, 0x3c, 0x48 (the second also 0x7a, 0x7c), data_8002ceb0_slot01
 *     and data_80055f44_slot01 (only if the second was allocated), the
 *     field_4e of data_8018f5a0, game_state.field_c8, field_4f, field_ab,
 *     field_09, field_2c, field_65, and the field_01 of player_left and
 *     player_right; and a stack rectangle and a stack copy of the palette
 *     (not compared as memory; the recorder copies both at the calls).
 *   Callees, all replaced by recorders (the setup functions run code of
 *     the game that would need contracts of their own; the loads reach the
 *     library):
 *       func_8011eb4c, func_801285e0, func_8011a744: no arguments, result
 *         0; at its first call each stores a new random value into the
 *         object's field_2ac, which the function must undo after
 *         func_8011a744 (the last of the three);
 *       func_80157fc4 (two arguments: a rectangle and a source pointer;
 *         the recorder copies the 2 words of the rectangle and 40 words
 *         from the source), func_80157d9c (one argument);
 *       func_8011f1e0 (no argument; results: two blocks, or 0 for either,
 *         one result per call in turn);
 *       func_80151020 (one), func_80125dc0 (four), func_80137220 (two).
 *     The log copies, at every call, the object, both allocated blocks,
 *     the players' first words, and the words of every global and
 *     game_state field the function writes, so that a write moved from
 *     before a call to after it is seen. The pointers to the rectangle
 *     and to the palette are stack addresses that differ between the two
 *     codes: their low two bits are logged and the words behind them.
 *   Aliasing: the object, the two allocated blocks, the record behind
 *     data_8018f5a0 and the players are distinct blocks.
 *   Excluded inputs: none.
 *   Not reached by any input: the 22 instruction slots at offsets 0x3c to
 *     0x90 of the original, the byte-wise (lwl/lwr) copy of the palette
 *     into the stack frame. The original takes it only when the frame
 *     address or the source address is not word aligned; the source is a
 *     fixed word-aligned address of the module image and the stack
 *     pointer is 8-aligned by the calling convention, so no input
 *     reaches it. The other 207 slots are executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002ceb0_slot01;
extern Object *data_80055f44_slot01;
extern int data_80055f3c_slot01;
extern u8 data_80015174_slot01[];
extern u8 data_80070d48[];
extern HudState *data_8018f5a0;
Block172 *func_8011f1e0(void);

void func_80010104_slot01(void) {
    u16 pal[80] = {
        0x0000, 0x5840, 0x6040, 0x68c0, 0x7900, 0x7980, 0x79c0, 0x7a04, 0x7a46, 0x7a8a, 0x7acc, 0x7b10, 0x7b54, 0x7b96, 0x7bda, 0x7bde,
        0x0000, 0x2816, 0x3058, 0x3098, 0x38da, 0x411c, 0x495e, 0x51de, 0x5a1e, 0x625e, 0x6a9e, 0x72de, 0x731e, 0x735e, 0x7b9e, 0x7bde,
        0x0000, 0x1980, 0x19c2, 0x2202, 0x2244, 0x2a86, 0x32c8, 0x330a, 0x3b4c, 0x438e, 0x4bd0, 0x63d4, 0x6bd6, 0x73d8, 0x7bda, 0x7bde,
        0x0000, 0x015a, 0x019c, 0x01dc, 0x021e, 0x025e, 0x029e, 0x1ade, 0x231e, 0x2b5e, 0x339e, 0x3bde, 0x53de, 0x63de, 0x6bde, 0x7bde,
        0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000};
    Rect rect;
    Object *other = game_state.field_78;
    int saved;
    Object *first;
    Object *second;

    data_80055f3c_slot01 = game_state.field_40;
    data_801ac6a8[0] = 0;
    data_801aa5ea[0] = 0;
    game_state.field_40 = data_801abf08;
    saved = other->field_2ac;
    data_80190568 = 1;
    func_8011eb4c();
    if (data_801abf08 != 0x12) {
        func_801285e0();
    }
    func_8011a744();
    other->field_2ac = saved;
    rect.x = 0x70;
    rect.y = 0x1f0;
    rect.w = 0x10;
    rect.h = 2;
    func_80157fc4(&rect, data_80015174_slot01);
    func_80157d9c(0);
    if (data_80197f10 >= 2) {
        rect.x = 0x70;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 0xb;
        func_80157fc4(&rect, data_80070d48);
        func_80157d9c(0);
        rect.x = 0x40;
        rect.y = 0x1f0;
        rect.w = 0x10;
        rect.h = 5;
        func_80157fc4(&rect, (u8 *)pal);
        func_80157d9c(0);
        first = (Object *)func_8011f1e0();
        if (first) {
            first->field_00 = 1;
            first->field_02 = 0x95;
            first->field_03 = 0;
            first->field_48 = other->field_65;
            first->field_3c = other;
        }
        second = (Object *)func_8011f1e0();
        data_8002ceb0_slot01 = second;
        if (second) {
            second->field_00 = 1;
            second->field_02 = 0x1b;
            second->field_03 = 0;
            second->field_48 = other->field_65;
            second->field_3c = other;
            second->field_7a = 0x70;
            second->field_7c = 0x1e0;
            data_80055f44_slot01 = second;
        }
        data_8018f5a0->field_4e++;
        game_state.field_c8 = 0x30;
        game_state.field_4f = 0;
        game_state.field_ab = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
        game_state.field_09 = 1;
        game_state.field_2c = 0xff;
        game_state.field_65 = 1;
        func_80151020(0x605);
        func_80125dc0(0, 0x20, 0xd, 0);
        func_80125dc0(1, 0x20, 0xd, 0);
        func_80125dc0(2, 0x20, 0xd, 0);
        func_80125dc0(3, 0x20, 0xd, 0);
        func_80137220(0, 6);
        func_80137220(1, 0);
        func_80137220(2, 1);
        func_80137220(3, 2);
    }
}
