/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred, not an original name): appends one 20-byte
   command record to the command buffer and hands it to func_8015bf34. The
   low nibble of c, moved to the high nibble, goes into the record's byte
   0xc and the high nibble of c into byte 0xd; d and e are stored as two
   halfwords, data_8018d208 as a third, and two fields are set to 0x10.
   Nothing happens when the low byte of c is 0.
   The tree declares the function with a pointer first argument, an int
   third and an s16 fifth; c is narrowed to a byte at entry and a is
   passed on as a number. The incoming value of the second argument takes
   no part in what the function does. */
void func_80152124(u8 *a, int b, int arg_c, s16 d, s16 e) {
    u8 c = arg_c;

    if (c != 0) {
        Cmd *cmd = data_8018d13c;
        u16 dv = data_8018d208;
        int lo;

        lo = c & 0xf;
        lo <<= 4;
        cmd->field_0c = lo;
        /* Stand-in, not recovered source: the high nibble kept in the parameter b, with a term ORed to it that is 0 for every value of b, reproduces the original's choice of registers. Without it this function differs from the original in 7 instruction slots. */
        /* The original has the high nibble in the second argument register while the low one and the loaded halfword are in the first two result registers. The compiler gives a value that is used in one block the first free register, and that is another one here. The parameter is a value of more than one block as long as its incoming value is read, and such a value gets its register in a later pass, which prefers the register it arrived in. The term is that read. It is 0 because the shift leaves the low 24 bits clear, and it leaves no instruction of its own. This reading of the compiler is inferred from its passes' output. */
        /* The figure above is with `b = c & 0xf0;` in place of the statement below. With a local in place of the parameter and no term it is the same, 7 slots; with a local and the term, 14 slots and 136 bytes. */
        b = (c & 0xf0) | (((u32)b << 24) & 0x100);
        cmd->field_0d = b;
        cmd->field_0e = dv;
        cmd->field_08 = d;
        cmd->field_0a = e;
        cmd->field_10 = 0x10;
        cmd->field_12 = 0x10;
        func_8015bf34((int)a, cmd++);
        data_8018d13c = cmd;
    }
}
