/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/*
 * What it does (inferred, not an original name): starts a palette
 * sequence. It stores the sequence record in the entry, splits the
 * record's header word into the entry's two halfwords (high half to
 * field_04, low half to field_06), computes a destination from the entry's
 * field_08 and field_0a (a position, read as a row and column of a
 * palette area), and copies the record's data there with func_8011fae0.
 * The same code is func_8011f994 of the resident image, which takes the
 * destination base as an argument; here the base is the pointer word
 * data_801aa624.
 */
void func_801e9c80_slot06_0e(Slot06Pal *pal, SeqRec *rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int offset, q;
    u8 *base;
    int t;
    int v;

    pal->rec = rec;
    /* header is copied through t and v; with one local for both the function differs from the original in 18 instruction slots (and is 4 bytes shorter). */
    t = rec->header;
    v = t;
    t = v >> 16;
    /* Written through a cast pointer; the plain struct store differs from the original in 3 instruction slots (order of instructions: the delay slot). */
    *(s16 *)((u8 *)pal + 4) = t;
    pal->field_06 = v;
    /* Stand-in, not recovered source: one stand-in at 3 places, the term that is ORed to the field_08 bits below, which is 0 for every value of v, reproduces the original's choice of registers. Without it (all three taken out) this function differs from the original in 10 instruction slots. */
    q = ((pal->field_08 & 0xe00) | (((u32)v << 24) & 0x100) |
         /* The second place of the stand-in above. The figure is the one above: the three terms taken out. */
         (((u32)v << 25) & 0x200) |
         /* The third place of the stand-in above. The figure is the one above: the three terms taken out. */
         (((u32)v << 26) & 0x400)) * 4;
    /* base is a local of its own; written inline in the call, the function differs from the original in 9 instruction slots. */
    base = data_801aa624;
    offset = (pal->field_08 & 0x1f0) / 16 * 4 + q + (((pal->field_0a / 16) << 23) >> 16);
    func_8011fae0(rec->data, (u16 *)(base + ((offset << 15) >> 15)));
}
