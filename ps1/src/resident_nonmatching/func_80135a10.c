/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in the order of two
 * instructions. The PS1 build keeps the original bytes of the resident
 * executable for it and does not use this file. The differential test next
 * to it (difftest.py with func_80135a10.py) compares the behavior of this C
 * with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): updates the strips of the
 * two player objects. Mode 1 of data_80190474: the object that
 * game_state.field_78 points at (when not null; it may be one of the two
 * players or a separate block) has its field_65 select player_left (0) or
 * the next object (not 0); its field_167 is stored in that player's byte
 * table at the index field_ce - 1, the player is passed to func_80120554
 * with the player number and 0x34c, and the two strips of that index (one
 * in each row of the player's pair of rows) are passed to func_80135c0c
 * with a code made from field_167: shifted left by two and cut to a byte
 * (so bit 6 of field_167 is lost), with bit 1 of the code set when bit 7
 * of field_167 is set. Mode 2: for each player and each of the four table
 * entries that holds 8, the two strips of that index are passed to
 * func_80135c0c with code 0x20. Any other mode does nothing.
 *
 * Contract (the roles named for the fields are inferred):
 *   No argument, no return value.
 *   Reads: data_80190474 (mode); in mode 1 game_state.field_78 and, from the
 *     object it points at, field_65, field_ce and field_167; in mode 2 the
 *     byte tables bytes_d0[0..3] of player_left and of the object after it.
 *   Writes: in mode 1 one byte of bytes_d0 of the selected player (at the
 *     index field_ce - 1, 0 to 255: the byte lies inside the player object,
 *     which the test fills whole with random bytes).
 *   Calls (replaced by recorders in the test, both runs alike):
 *     func_80120554(object, player number, 0x34c), three arguments;
 *     func_80135c0c(strip, strip, code), three arguments. Both return 0.
 *   Watched by the test at every call: player_left and the object after it,
 *     whole (0x394 bytes each); the player passed to func_80120554 is also
 *     copied as that call's pointee.
 *   Aliasing: the object that field_78 points at may be player_left, the
 *     object after it, or a separate block; the test uses all three. In mode 1
 *     with a separate block field_65 still selects the player written.
 *   Excluded: none. field_ce - 1 is a byte and may be any value 0 to 255;
 *     the byte tables and strips are then indexed past their ends, which
 *     stays inside RAM and is the same in the original (inferred).
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80135c0c(u8 *a, u8 *b, u8 index);

void func_80135a10(void) {
    Object *left = &player_left;
    Object *right = &player_left + 1;
    Object *obj;
    u8 v;
    u8 i;

    if (data_80190474 == 1) {
        obj = game_state.field_78;
        if (obj != 0) {
            v = obj->field_167;
            i = obj->field_ce - 1;
            if (obj->field_65 == 0) {
                left->bytes_d0[i] = v;
                func_80120554(left, 0, 0x34c);
                if (v & 0x80) {
                    v = ((v & 0x7f) << 2) | 2;
                } else {
                    v = v << 2;
                }
                func_80135c0c((u8 *)&strips[0][i], (u8 *)&strips[1][i], v);
            } else {
                right->bytes_d0[i] = v;
                func_80120554(right, 1, 0x34c);
                if (v & 0x80) {
                    v = ((v & 0x7f) << 2) | 2;
                } else {
                    v = v << 2;
                }
                func_80135c0c((u8 *)&strips[2][i], (u8 *)&strips[3][i], v);
            }
        }
    } else if (data_80190474 == 2) {
        for (i = 0; i < 4; i++) {
            if (left->bytes_d0[i] == 8) {
                func_80135c0c((u8 *)&strips[0][i], (u8 *)&strips[1][i], 0x20);
            }
        }
        for (i = 0; i < 4; i++) {
            if (right->bytes_d0[i] == 8) {
                func_80135c0c((u8 *)&strips[2][i], (u8 *)&strips[3][i], 0x20);
            }
        }
    }
}
