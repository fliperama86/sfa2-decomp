/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C differs from the original's bytes (the printed line of the
 * test gives both sizes). The build does not use this file. The differential
 * test next to it (difftest.py, with func_80166144.py as the contract)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * Provenance: written by this project from the listing of the original. The
 * sound library of the reference that the SDK files come from has no
 * SsSepOpen in its source; the single-sequence twin SsSeqOpen of
 * ../sdk/libsnd/seqinit.c was read as an aid, and none of its text is copied.
 *
 * What it does (inferred, not an original name; the library's name for this
 * address is probably SsSepOpen): opens a group of count sequences that lie
 * one after another at addr. Looks for the first clear bit of the word
 * _snd_openflag; when all 32 bits are set it prints "Can't Open Sequence
 * data any more" through printf and returns -1. Otherwise it sets that bit
 * (the bit number is the slot, an s16) and, for each index 0 to count - 1,
 * calls func_80165d84 (inferred _SsInitSoundSep) with the slot, the index,
 * vab_id and the current address; that function returns the number of bytes
 * that sequence takes (inferred), or -1. A result of -1 ends the loop and
 * the function returns -1 (the bit stays set); any other result is added to
 * the address. After the loop it returns the slot.
 *
 * Contract:
 *   Arguments: a0 = addr (a byte pointer), a1 = vab_id and a2 = count; both
 *     are taken as s16 (the original sign-extends the low half of each; the
 *     prototype in protos.h declares them int, which this definition keeps).
 *   Reads: _snd_openflag. addr is not read here, only passed on and moved.
 *   Writes: _snd_openflag (the slot's bit is set when a slot is found).
 *   Callees: printf and func_80165d84 are replaced by recorders in both
 *     runs. The recorder of printf logs its address and the first 36 bytes
 *     of its string (9 words behind the pointer; the pointer itself is
 *     logged under a mask of 0, the strings sit at different addresses in
 *     the two builds). The recorder of func_80165d84 takes four arguments,
 *     logs the slot, the index and vab_id under a mask of 0xffff and the
 *     address in full, and returns results from a list the setup makes (a
 *     sequence length, or -1); what the real callee does to memory is
 *     outside the test.
 *   Aliasing: none; addr is never dereferenced here.
 *   Excluded inputs: none other than the above. The case where the
 *     recorder's last result repeats is the contract's own: after the list
 *     is used up, its last value is returned again.
 *   Not reached by any input: none known.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 _snd_openflag;

int printf(const char *, ...);
int func_80165d84(s16 slot, s16 index, s16 vab_id, u8 *addr);

int func_80166144(u8 *addr, int vab_id, int count) {
    s16 seq_sep_no;
    u32 bit_pos;
    u8 found;
    s16 i;
    int len;

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
        len = func_80165d84(seq_sep_no, i, vab_id, addr);
        if (len == -1) {
            return -1;
        }
        addr += len;
    }
    return seq_sep_no;
}
