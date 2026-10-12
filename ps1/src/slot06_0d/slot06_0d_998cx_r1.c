/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/*
 * What it does (inferred, not an original name): starts a palette
 * sequence. It stores the sequence record in the entry, splits the
 * record's header word into the entry's two halfwords (high half to
 * field_04, low half to step_value), computes a destination from the entry's
 * rect_x and rect_y (a position, read as a row and column of a
 * palette area), and copies the record's data there with copy_block_pairs_wide.
 * The same code is set_sequence_and_copy_block of the resident image, which takes the
 * destination base as an argument; here the base is the pointer word
 * data_801aa624.
 */
void set_palette_record_and_copy_data_slot06_0d(Slot06Pal *pal, SeqRec *rec) {
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
    pal->step_value = v;
    /* Stand-in, not recovered source: one stand-in at 3 places, the term that is ORed to the rect_x bits below, which is 0 for every value of v, reproduces the original's choice of registers. Without it (all three taken out) this function differs from the original in 10 instruction slots. */
    q = ((pal->rect_x & 0xe00) | (((u32)v << 24) & 0x100) |
         /* The second place of the stand-in above. The figure is the one above: the three terms taken out. */
         (((u32)v << 25) & 0x200) |
         /* The third place of the stand-in above. The figure is the one above: the three terms taken out. */
         (((u32)v << 26) & 0x400)) * 4;
    /* base is a local of its own; written inline in the call, the function differs from the original in 9 instruction slots. */
    base = data_801aa624;
    offset = (pal->rect_x & 0x1f0) / 16 * 4 + q + (((pal->rect_y / 16) << 23) >> 16);
    copy_block_pairs_wide(rec->data, (u16 *)(base + ((offset << 15) >> 15)));
}
