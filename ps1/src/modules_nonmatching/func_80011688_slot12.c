/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs in register choice and instruction order. The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the module
 * image; the build does not use this file. The differential test next to it
 * (difftest.py, with func_80011688_slot12.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): fades a 16-entry palette
 * one step toward a target palette. data_80028aa0_slot12 is the target (16
 * halfwords of 5-5-5 colour), the table that data_80028b40_slot12 points at
 * is the current palette. For each entry that differs, each of the three
 * 5-bit channels moves up by 2, but not past the target's channel; the
 * entry is rewritten. The 16 entries are then loaded to the frame buffer
 * rectangle (x 16, y 480, 16 wide, 1 high) and the function waits for the
 * drawing to finish. It returns 1 when any entry differed, else 0.
 *
 * Contract:
 *   No argument. Returns int (v0).
 *   Reads: the target palette (16 halfwords) and, through the pointer word
 *     data_80028b40_slot12, the current palette (16 halfwords).
 *   Writes: the current palette entries that differ from the target. The
 *     channel in bits 10 to 14 is taken from bit 10 up (6 bits, as the
 *     original does not mask it), and a result is truncated to 16 bits.
 *   Callees, both in Sony's library, replaced by recorders:
 *     func_80157fc4 (2 arguments: a rectangle of 4 halfwords on the stack,
 *     the pixel pointer; the log copies the 2 words of the rectangle and the
 *     8 words of the palette it points at) and func_80157d9c (1 argument,
 *     0). Their results are unused.
 *   Aliasing: the target palette and the current palette are distinct.
 *   Not reached: nothing is excluded; every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80028aa0_slot12[];
extern u16 *data_80028b40_slot12;

int func_80011688_slot12(void) {
    Rect r;
    int i;
    unsigned blue_target, green_target, red_target;
    unsigned blue, green, red;
    u16 *target = (u16 *)data_80028aa0_slot12;
    u16 *cur = data_80028b40_slot12;
    int changed = 0;

    for (i = 0; i < 16; i++, target++, cur++) {
        if (*cur != *target) {
            changed = 1;
            blue_target = *target >> 10;
            green_target = (*target >> 5) & 0x1f;
            red_target = *target & 0x1f;
            blue = (*cur >> 10) + 2;
            green = ((*cur >> 5) & 0x1f) + 2;
            red = (*cur & 0x1f) + 2;
            if (blue >= blue_target) blue = blue_target;
            if (green >= green_target) green = green_target;
            if (red >= red_target) red = red_target;
            *cur = (blue << 10) | (green << 5) | red;
        }
    }
    r.x = 0x10;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 1;
    func_80157fc4(&r, (u8 *)data_80028b40_slot12);
    func_80157d9c(0);
    return changed;
}
