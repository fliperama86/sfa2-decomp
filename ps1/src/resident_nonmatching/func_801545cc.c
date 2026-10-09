/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the executable; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): prepares the tables of a
 * two-column selection screen. First it advances a counter of the HUD state
 * (game_state.field_30 not 0), or calls func_801519b4 with one of two
 * blocks (chosen by game_state.field_31) and, if the player selected by
 * field_31 has bit 0x100 in field_c2, advances the counter and calls
 * func_80120554. Then it clears five cursor bytes, sets data_8018d264 to 1,
 * copies two rows of 8 ids from table_8016e664 into the second byte of 8
 * five-byte entries of each of two tables, writes a kind byte (0x1a for the
 * ids 0x94 to 0x97, else 0x1d) into the first byte of each of those entries,
 * and marks the cursor entries of the four small tables (0x14 for the
 * current position, 0x1a for the first entries, 0x1b for the rest).
 *
 * Contract:
 *   Argument: none. No return value.
 *   Reads: game_state.field_30 and field_31 (bytes), player_left.field_c2 and
 *     player_right.field_c2 (halfwords), the pointer data_8018f5a0 and the
 *     halfword at its offset 0x52, the 16 bytes at table_8016e664 + 0x10 and
 *     + 0x18 (8 bytes each).
 *   Writes: the halfword at data_8018f5a0 + 0x52 (incremented once or not at
 *     all), the bytes data_8018d250, d254, d258, d25c, d260 (0) and
 *     data_8018d264 (1), and bytes in the tables data_8018125e / 8018128a
 *     (entries of 5 bytes, 8 of them), data_80181094 / data_80181138
 *     (entries of 14 bytes), data_801811aa and data_80181206 (16 bytes),
 *     table_801811ed (11 bytes), data_8018124a (9 bytes).
 *   Aliasing: the tables, the HUD state block and the byte tables of the
 *     image are distinct areas; the HUD block is a block of its own.
 *   Callees: func_801519b4 (one argument) and func_80120554 (three
 *     arguments) are replaced by recorders; both reach Sony's library. The
 *     log watches the HUD block, data_8018d250 to d268 and the tables from
 *     data_80181094 on, at every call. In half of the cases the recorder of
 *     func_801519b4 stores a new field_31 at its first call (the callee may
 *     do so; inferred), so the reread after the call is tested; what the
 *     callees do otherwise is outside the test.
 *   Indices: the original reads data_8018d250, d254, d260 and d264 back as
 *     the indices of the cursor marks; they have just been set to 0 and 1,
 *     so this C writes the marks at the constant indices.
 *   Not reached: none planned; the coverage line says.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_8018d260;
extern u8 data_801811aa[];
extern u8 data_80181206[];
extern u8 data_8018124a[];
extern u8 data_80181044[];
extern u8 data_80181060[];

void func_801545cc(void) {
    int i;
    int id;
    u8 *block;
    u8 *ids = (u8 *) table_8016e664 + 0x10; /* inferred: two runs of 8 ids past the declared table */

    if (game_state.field_30 == 0) {
        block = data_80181060;
        if (game_state.field_31 == 1) {
            block = data_80181044;
        }
        func_801519b4((Object *) block);
        if ((game_state.field_31 == 1 && (player_left.field_c2 & 0x100)) ||
            (game_state.field_31 == 2 && (player_right.field_c2 & 0x100))) {
            data_8018f5a0->field_52++;
            func_80120554(0, 0, 0x34c);
        }
    } else {
        data_8018f5a0->field_52++;
    }

    data_8018d250 = 0;
    data_8018d254 = 0;
    data_8018d258 = 0;
    data_8018d25c = 0;
    data_8018d260 = 0;
    data_8018d264 = 1;

    for (i = 0; i < 8; i++) {
        data_8018125e[i * 5 + 1] = ids[i];
        data_8018128a[i * 5 + 1] = ids[i + 8];
    }
    for (i = 0; i < 8; i++) {
        id = data_8018125e[i * 5 + 1];
        data_8018125e[i * 5] = (id >= 0x94 && id <= 0x97) ? 0x1a : 0x1d;
        id = data_8018128a[i * 5 + 1];
        data_8018128a[i * 5] = (id >= 0x94 && id <= 0x97) ? 0x1a : 0x1d;
    }

    data_80181094[1] = 0x1a;
    data_80181138[1] = 0x1a;
    data_801811aa[0] = 0x14;
    table_801811ed[11] = 0x14;
    data_80181206[0] = 0x14;
    data_8018124a[0] = 0x14;
    for (i = 1; i < 8; i++) {
        data_80181094[i * 14 + 1] = 0x1b;
        data_80181138[i * 14 + 1] = 0x1b;
    }
    data_801811aa[16] = 0x1a;
    data_801811aa[32] = 0x1a;
    table_801811ed[0] = 0x1a;
    data_80181206[16] = 0x1a;
    data_80181206[32] = 0x1a;
    data_80181206[48] = 0x1a;
    data_8018124a[9] = 0x1a;
}
