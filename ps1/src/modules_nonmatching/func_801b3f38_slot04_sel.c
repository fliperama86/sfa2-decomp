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
 * a two-player selection screen. It calls func_801519b4 for two text
 * buffers, then runs for each player a handler picked from a table by the
 * first byte of the player's selection record (records of 0x15 bytes at
 * data_801b9d38_slot04_sel, one per player, the handler gets the player
 * object and the record). Then, when game_state.field_07 is not 0 and the
 * field_0d of either record is not 0, it clears both records' first byte,
 * adds one to the hud state's field_4e and clears its field_50. Finally,
 * when the byte data_801b9d68_slot04_sel differs from game_state.field_07
 * or data_801b9d6c_slot04_sel from game_state.mode, it clears both records'
 * first byte and the hud state's field_50.
 *
 * Contract (what the code reads and writes):
 *   Argument: none. No return value.
 *   Reads: game_state.field_07 and mode, the two selection records (first
 *     byte, field_0d), the bytes data_801b9d68_slot04_sel and
 *     data_801b9d6c_slot04_sel, the pointer data_8018f5a0 and the hud
 *     state's field_4e.
 *   Writes: the first byte of both records, the hud state's field_4e and
 *     field_50.
 *   Calls through the table data_801b7df0_slot04_sel: the original table in
 *     the module image has one entry (index 0, a function of this image);
 *     the bytes after it are not pointers, so only first byte 0 is valid in
 *     the original (read from the original's listing, not tested). The
 *     contract writes four entries (indices 0 to 3), each
 *     the address of a recorder of its own (2 arguments, return 0), and
 *     gives the records' first bytes in 0 to 3, so that clearing a first
 *     byte is a difference. The log watches both records (11
 *     words; the second argument points into them, at an address that is
 *     not word aligned, so it is not a pointee), the hud state block (the
 *     pointer data_8018f5a0 is set to a block of 0x64 bytes) and the word
 *     at data_801b9d68_slot04_sel (4 bytes, the first of which is the
 *     compared byte; data_801b9d6c_slot04_sel is not in it).
 *   Callee replaced by a recorder: func_801519b4 (1 argument; draws
 *     text, inferred), return 0.
 *   Aliasing: the records, hud block, text buffers and the player objects
 *     are distinct; the players are the game's player_left and
 *     player_right, only passed on.
 *   Excluded inputs: records' first bytes of 4 and above (the table beyond
 *     the first entry is not made of pointers).
 *   Not reached by any input: none expected (see the coverage line).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations, not original ones. */
extern HudState *data_8018f5a0;
extern TextBuf data_801b8638_slot04_sel;
extern TextBuf data_801b874c_slot04_sel;
extern Slot04SelRec9d38 data_801b9d38_slot04_sel[];
extern u8 data_801b9d68_slot04_sel;
extern u8 data_801b9d6c_slot04_sel;
extern void (*data_801b7df0_slot04_sel[])(Object *, Slot04SelRec9d38 *);

void func_801b3f38_slot04_sel(void) {
    Slot04SelRec9d38 *rec = data_801b9d38_slot04_sel;

    func_801519b4((Object *)&data_801b8638_slot04_sel);
    func_801519b4((Object *)&data_801b874c_slot04_sel);
    data_801b7df0_slot04_sel[rec[0].field_00](&player_left, &rec[0]);
    data_801b7df0_slot04_sel[rec[1].field_00](&player_right, &rec[1]);
    if (game_state.field_07 != 0) {
        if (rec[0].field_0d != 0 || rec[1].field_0d != 0) {
            rec[0].field_00 = 0;
            rec[1].field_00 = 0;
            data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
            data_8018f5a0->field_50 = 0;
        }
    }
    if (data_801b9d68_slot04_sel != game_state.field_07 || data_801b9d6c_slot04_sel != game_state.mode) {
        rec[0].field_00 = 0;
        rec[1].field_00 = 0;
        data_8018f5a0->field_50 = 0;
    }
}
