/* Reconstruction, written by this project from the listing of the original:
   the reference that the other files of this folder come from has no such
   function. Names/roles inferred, not original symbols. The library's name
   for this function is probably SsSepOpen; the single-sequence twin SsSeqOpen
   of seqinit.c was read as an aid, and none of its text is copied. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u32 _snd_openflag;

int printf(const char *, ...);
int func_80165d84(s16 slot, s16 index, s16 vab_id, u8 *addr);

/* What it does (inferred): opens a group of count sequences that lie one
   after another at addr. It looks for the first clear bit of the word
   _snd_openflag; when all 32 bits are set it prints a message and returns -1.
   Otherwise it sets that bit (the bit number is the slot) and, for each index
   0 to count - 1, calls func_80165d84 (inferred _SsInitSoundSep) with the
   slot, the index, vab_id and the current address; that function returns the
   number of bytes the sequence takes (inferred), or -1. A result of -1 ends
   the loop and the function returns -1 (the bit stays set); any other result
   is added to the address. After the loop it returns the slot. */
int func_80166144(u8 *addr, int vab_id, int count) {
    s16 seq_sep_no;
    u32 bit_pos;
    u8 found;
    s16 i;
    int len;
    /* The address walks in a local of its own: walked in the parameter, this function differs from the original in 4 instruction slots. */
    u8 *p = addr;

    if (_snd_openflag == 0xFFFFFFFF) {
        printf("Can't Open Sequence data any more\n\n");
        return -1;
    }
    bit_pos = 0;
    found = 0;
    while (!found) {
        if (!((1 << bit_pos) & _snd_openflag)) {
            seq_sep_no = bit_pos;
            found = 1;
        }
        bit_pos++;
    }
    _snd_openflag |= 1 << seq_sep_no;
    for (i = 0; i < (s16)count; i++) {
        len = func_80165d84(seq_sep_no, i, vab_id, p);
        if (len == -1) {
            return -1;
        }
        p += len;
    }
    return seq_sep_no;
}
