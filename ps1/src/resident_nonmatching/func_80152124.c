/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice (read from the original's listing, not tested). The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the
 * executable; the build does not use this file. The differential test next
 * to it (difftest.py) compares the behavior of this C with the original
 * code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): appends one 20-byte
 * command record to the command buffer and hands it to func_8015bf34. The
 * low nibble of c, moved to the high nibble, goes into the record's byte
 * 0xc and the high nibble of c into byte 0xd; d and e are stored as two
 * halfwords, data_8018d208 as a third, and two fields are set to 0x10.
 * Nothing happens when the low byte of c is 0.
 *
 * Contract:
 *   Arguments: a0 = a, passed on to the callee; a1 unused; a2 = c (only the
 *     low byte is used); a3 = d (halfword); e is the fifth argument, a
 *     halfword read from the caller's stack. No return value.
 *   Reads: the buffer pointer data_8018d13c and the halfword data_8018d208.
 *   Writes: the record at the buffer pointer (offsets 8, 0xa, 0xc, 0xd, 0xe,
 *     0x10, 0x12; the other bytes are left alone) and the buffer pointer,
 *     advanced by 0x14. Both only when the low byte of c is not 0.
 *   Callee: func_8015bf34(a, record) is replaced by a recorder (it is in
 *     the code that follows Sony's library); its result is not used. The
 *     log copies the 5 words of the record at each call, and watches the
 *     buffer block and the buffer pointer: the record is complete before the
 *     call and the pointer is advanced after it.
 *   Aliasing: the record lies in a block of its own.
 *   Not reached: none; every instruction slot of the original is executed
 *     (inferred).
 * The tree declares func_80152124 with a pointer first argument, an int third and an s16 fifth; c is narrowed to a byte at entry and a is passed on as a number.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80152124(u8 *a, int b, int arg_c, s16 d, s16 e) {
    u8 c = arg_c;
    Cmd *cmd;

    if (c != 0) {
        cmd = data_8018d13c;
        cmd->field_0c = (c & 0xf) << 4;
        cmd->field_0d = c & 0xf0;
        cmd->field_0e = data_8018d208;
        cmd->field_08 = d;
        cmd->field_0a = e;
        cmd->field_10 = 0x10;
        cmd->field_12 = 0x10;
        func_8015bf34((int)a, cmd);
        data_8018d13c = cmd + 1;
    }
}
