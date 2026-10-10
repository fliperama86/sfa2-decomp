/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice. The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the
 * module image; the build does not use this file. The differential test
 * next to it (difftest.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): starts a palette
 * sequence. It stores the sequence record in the entry, splits the
 * record's header word into the entry's two halfwords (high half to
 * field_04, low half to field_06), computes a destination from the entry's
 * field_08 and field_0a (a position, read as a row and column of a
 * palette area), and copies the record's data there with func_8011fae0.
 * The same code is func_8011f994 of the resident image, which takes the
 * destination base as an argument; here the base is the pointer word
 * data_801aa624.
 *
 * Contract (the roles named for the fields are inferred):
 *   Arguments: a0 = pal, a1 = rec. No return value.
 *   Reads: rec->header and rec->data; pal->field_08 (bits 4 to 8 and 9 to
 *     11 only) and pal->field_0a (signed, divided by 16 and cut to its low
 *     nine bits); the pointer word data_801aa624, which is the base.
 *   Writes: pal->rec, pal->field_04, pal->field_06, and what func_8011fae0
 *     writes at base + offset, offset being a 17-bit signed value.
 *   Callee: func_8011fae0 is game code and runs as the original in both
 *     runs. It reads a width and a height (halfwords) at rec->data and
 *     copies pairs of halfwords from there, a row of width pairs per
 *     step with a stride of 0x80 bytes in the destination.
 *   Excluded inputs: the width and height at rec->data are chosen small
 *     (at most 16 and 8) and the base is chosen so that every destination
 *     the offset can reach lies in the block the setup made; the original
 *     writes wherever the pointers lead, so it needs this.
 *   Aliasing: pal, rec, the data and the destination area are distinct
 *     blocks.
 *   Slots no input reaches: the single instruction that adds 0xf to the
 *     masked field_08 when that value is negative; the value is masked
 *     with 0x1f0 and cannot be negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9c80_slot06_0e(Slot06Pal *pal, SeqRec *rec) {
    int header;
    int offset;

    pal->rec = rec;
    header = rec->header;
    pal->field_04 = header >> 16;
    pal->field_06 = header;
    offset = (pal->field_08 & 0x1f0) / 16 * 4
           + (pal->field_08 & 0xe00) * 4
           + (((pal->field_0a / 16) << 23) >> 16);
    func_8011fae0(rec->data, (u16 *)(data_801aa624 + ((offset << 15) >> 15)));
}
