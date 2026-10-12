/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern u16 data_801a6966;

/*
 * What it does (inferred): decodes a code word of the player's input log.
 * The side of the object game_state.focus_object selects the word data_801a6972
 * (side not 0) or data_801a6966; its low byte goes to data_80185fc8 and its
 * top nibble to data_80185fcc. The low byte picks a base value for
 * data_80185fd0 (0x94 and 0x90: 0; 0x68 and 0x60: 4; 0xc0: 8; 0x30: 0xc;
 * 6: 0x10; 9: 0x14); the byte 0 or any other value fails. A known byte
 * indexes data_8016e9a8 with the nibble (masked to 4 bits); a negative entry
 * fails. Success increments input_step_index and sets input_match_offset; failure clears
 * input_step_index and input_match_offset and sets input_timer to 0x3c.
 */
void check_input_code_step(void) {
    u16 *code;
    u8 byte;
    u16 nib;
    if (game_state.focus_object->side) {
        code = &data_801a6972;
        byte = *code & 0xff;
        nib = *code;
    } else {
        code = &data_801a6966;
        byte = *code & 0xff;
        nib = *code;
    }
    /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        data_80185fc8 = byte;
        data_80185fcc = nib >> 12;
    } while (0);
    /* The byte 0 has a test of its own, as in the original: without it (the last arm of the chain below also catches 0) this function differs from the original in 19 instruction lines. */
    if (data_80185fc8 == 0) {
        goto fail;
    }
    /* Each arm stores its own constant to data_80185fd0: with one store after the chain through a local, this function differs from the original in 29 instruction lines. */
    if (data_80185fc8 == 0x94 || data_80185fc8 == 0x90) {
        data_80185fd0 = 0;
    } else if (data_80185fc8 == 0x68) {
        data_80185fd0 = 4;
    } else if (data_80185fc8 == 0x60) {
        data_80185fd0 = 4;
    } else if (data_80185fc8 == 0xc0) {
        data_80185fd0 = 8;
    } else if (data_80185fc8 == 0x30) {
        data_80185fd0 = 0xc;
    } else if (data_80185fc8 == 6) {
        data_80185fd0 = 0x10;
    } else if (data_80185fc8 == 9) {
        data_80185fd0 = 0x14;
    } else {
        goto fail;
    }
    data_80185fcc &= 0xf;
    data_80185fd4 = data_8016e9a8[data_80185fcc];
    if (data_80185fd4 >= 0) {
        game_state.input_step_index++;
        game_state.input_match_offset = data_80185fd0 + data_80185fd4;
        return;
    }
fail:
    game_state.input_step_index = 0;
    game_state.input_timer = 0x3c;
    game_state.input_match_offset = 0;
}
