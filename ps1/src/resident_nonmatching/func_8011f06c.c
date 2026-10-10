/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original forms the address of the first byte before it clears the loop
 * counter; the code built from this C clears the counter first (two
 * instruction slots, registers right). The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the resident image; the build does not
 * use this file. The differential test next to it (difftest.py) compares
 * the behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): clears 128 bytes of
 * game_state starting at field_40, then clears the byte at field_226.
 *
 * Contract:
 *   No argument, no result.
 *   Reads nothing. Writes game_state bytes 0x40 to 0xbf and byte 0x226
 *     (all set to 0); nothing else is written.
 *   No callees. Every instruction slot of the original is executed by
 *     every case.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f06c(void) {
    u8 *p = &game_state.field_40;
    int i;

    for (i = 0; i < 0x80; i++) {
        *p++ = 0;
    }
    game_state.field_226 = 0;
}
