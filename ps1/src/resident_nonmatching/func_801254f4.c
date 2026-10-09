/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes (the build is 16 bytes
 * shorter) in instruction order and in how the arms that store 4 are
 * shared. The build does not use this file. The differential test next to
 * it (difftest.py, with func_801254f4.py) compares the behavior of this C
 * with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): decodes a code word of the
 * player's input log. The side of the object game_state.field_78 selects
 * the word data_801a6972 (side not 0) or data_801a6966; its low byte goes
 * to data_80185fc8 and its top nibble to data_80185fcc. The low byte picks
 * a base value for data_80185fd0: 0x94 and 0x90 give 0, 0x68 and 0x60 give
 * 4, 0xc0 gives 8, 0x30 gives 0xc, 6 gives 0x10, 9 gives 0x14; the byte 0 or
 * any other value fails. For a known byte the nibble (masked to 4 bits and
 * stored back to data_80185fcc) indexes the signed byte table
 * data_8016e9a8, the entry goes to data_80185fd4, and a negative entry
 * fails. Success: game_state.field_138 is incremented and field_13a gets
 * the sum of data_80185fd0 and data_80185fd4 (as a byte). Failure:
 * field_138 and field_13a are cleared and field_139 gets 0x3c.
 *
 * Contract:
 *   No arguments, no return value.
 *   Reads: game_state.field_78 (pointer to an object, its side byte),
 *     data_801a6966, data_801a6972 (halfwords), data_8016e9a8 (16 signed
 *     bytes), game_state.field_138.
 *   Writes: data_80185fc8, data_80185fcc, and on a known byte data_80185fd0
 *     and data_80185fd4 (when the nibble is reached), game_state.field_138,
 *     field_139, field_13a.
 *   Aliasing: the object, the table and the two code words are distinct.
 *   Excluded: none.
 *   Slots no input can reach: none known.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack them (inferred types). */
extern u16 data_801a6966;
extern u16 data_80185fc8;
extern u16 data_80185fcc;
extern int data_80185fd0;
extern int data_80185fd4;
extern s8 data_8016e9a8[];

void func_801254f4(void) {
    u16 *code;

    if (game_state.field_78->side) {
        code = &data_801a6972;
    } else {
        code = &data_801a6966;
    }
    data_80185fc8 = *(u8 *)code;
    data_80185fcc = *code >> 12;

    switch (data_80185fc8) {
    case 0x94:
    case 0x90:
        data_80185fd0 = 0;
        break;
    case 0x68:
    case 0x60:
        data_80185fd0 = 4;
        break;
    case 0xc0:
        data_80185fd0 = 8;
        break;
    case 0x30:
        data_80185fd0 = 0xc;
        break;
    case 6:
        data_80185fd0 = 0x10;
        break;
    case 9:
        data_80185fd0 = 0x14;
        break;
    default:
        goto fail;
    }
    data_80185fcc = data_80185fcc & 0xf;
    data_80185fd4 = data_8016e9a8[data_80185fcc];
    if (data_80185fd4 >= 0) {
        game_state.field_138++;
        game_state.field_13a = data_80185fd0 + data_80185fd4;
        return;
    }
fail:
    game_state.field_138 = 0;
    game_state.field_139 = 0x3c;
    game_state.field_13a = 0;
}
