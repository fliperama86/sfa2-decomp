/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size, instruction
 * scheduling and register choice. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the per-frame update of
 * a two-player selection screen of this image. It calls func_801519b4 for
 * the text buffers chosen by each player's side byte (two tables, left
 * player first), sets two halfwords of a text block, calls func_801519b4
 * for it and for another text block, then runs for each player a handler
 * picked from a table by the first byte of the player's selection record
 * (records of 0x19 bytes at data_801b9cf0_slot04_sel; the handler gets the
 * player object and the record). Then, when game_state.field_07 is not 0
 * and equals the OR of the two records' field_09, and either record's
 * field_0b is not 0, it clears both records' first byte, clears the hud
 * state's field_50 and adds one to its field_4e. Finally, when the byte
 * data_801b9d28_slot04_sel differs from game_state.field_07 or
 * data_801b9d2c_slot04_sel from game_state.mode, it clears both records'
 * first byte and sets the hud state's field_50 to 1.
 *
 * Contract (what the code reads and writes):
 *   Argument: none. No return value.
 *   Reads: the side byte (field at 0xa6) of player_left and player_right,
 *     the pointers in the tables data_801b701c_slot04_sel and
 *     data_801b6fcc_slot04_sel (indexed by the side byte, any value 0 to
 *     255), game_state.field_07 and mode, the two records (first byte,
 *     field_09, field_0b), the bytes data_801b9d28_slot04_sel and
 *     data_801b9d2c_slot04_sel, the pointer data_8018f5a0 and the hud
 *     state's field_4e.
 *   Writes: the halfwords at offsets 4 and 6 of data_801b7978_slot04_sel,
 *     the first byte of both records, the hud state's field_4e and field_50.
 *   Calls through the table data_801b6e70_slot04_sel: the original table in
 *     the module image has three entries (functions of this image) and then
 *     data that is not made of pointers, so only first bytes 0 to 2 are
 *     valid in the original. The contract writes four entries (indices 0 to
 *     3), each the address of a recorder of its own (2 arguments, return
 *     0), and gives the records' first bytes in 0 to 3, so that clearing a
 *     first byte is a difference.
 *   Callee replaced by a recorder: func_801519b4 (1 argument, draws text),
 *     return 0. The log watches both records (13 words), the hud state
 *     block, and the first 16 bytes of data_801b7978_slot04_sel (the
 *     second argument of a handler points into the records at an address
 *     that is not word aligned, so it is not a pointee).
 *   Aliasing: the records, hud block and text blocks are distinct; the
 *     players are the game's player_left and player_right.
 *   Excluded inputs: records' first bytes of 4 and above.
 *   Not reached by any input: none expected (see the coverage line).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations, not original ones. */
extern TextItem *data_801b701c_slot04_sel[];
extern TextItem *data_801b6fcc_slot04_sel[];
/* The start of a text block: two halfwords are set here, the rest is the
   buffer's. */
typedef struct {
    u8 head[4];
    u16 field_04;
    u16 field_06;
} Slot04SelTextHead;

extern TextItem data_801b7978_slot04_sel;
extern Slot04SelRec7648 data_801b7648_slot04_sel;
extern void (*data_801b6e70_slot04_sel[])(Object *, Slot04SelRec *);
extern Slot04SelRec data_801b9cf0_slot04_sel[];
extern u8 data_801b9d28_slot04_sel;
extern u8 data_801b9d2c_slot04_sel;
extern HudState *data_8018f5a0;

void func_801b0ea0_slot04_sel(void) {
    Slot04SelRec *rec = data_801b9cf0_slot04_sel;

    func_801519b4((Object *)data_801b701c_slot04_sel[player_left.side]);
    func_801519b4((Object *)data_801b701c_slot04_sel[player_right.side]);
    func_801519b4((Object *)data_801b6fcc_slot04_sel[player_left.side]);
    func_801519b4((Object *)data_801b6fcc_slot04_sel[player_right.side]);
    ((Slot04SelTextHead *)&data_801b7978_slot04_sel)->field_04 = 0xa8;
    ((Slot04SelTextHead *)&data_801b7978_slot04_sel)->field_06 = 0x30;
    func_801519b4((Object *)&data_801b7978_slot04_sel);
    func_801519b4((Object *)&data_801b7648_slot04_sel);
    data_801b6e70_slot04_sel[rec[0].field_00](&player_left, &rec[0]);
    data_801b6e70_slot04_sel[rec[1].field_00](&player_right, &rec[1]);
    if (game_state.field_07 == (rec[0].field_09 | rec[1].field_09)) {
        if (game_state.field_07 != 0) {
            if (rec[0].field_0b != 0 || rec[1].field_0b != 0) {
                rec[0].field_00 = 0;
                rec[1].field_00 = 0;
                data_8018f5a0->field_50 = 0;
                data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
            }
        }
    }
    if (data_801b9d28_slot04_sel != game_state.field_07 || data_801b9d2c_slot04_sel != game_state.mode) {
        rec[0].field_00 = 0;
        rec[1].field_00 = 0;
        data_8018f5a0->field_50 = 1;
    }
}
