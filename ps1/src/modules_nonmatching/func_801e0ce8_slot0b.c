/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice and
 * instruction order (and the two arms are one loop here). The exact owner
 * of the bytes in the PS1 build stays the raw bytes of the module image;
 * the build does not use this file. The differential test next to it
 * (difftest.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): fills a run of 0x28-byte
 * polygon records with the quads of a layout, twice in a row. A layout
 * reached through the object's sequence holds a count, a list of tile ids
 * and a list of coordinate pairs. For each entry it initialises the record
 * through func_8015c09c, then sets the three colour bytes to 0x80, a
 * texture word from the argument `tag`, the tile word from `base` plus the
 * id times 64, and the four corner positions: the pair plus (0x60, 0x50),
 * and the corners 0x10 further right and down. With `mirror` not 0 the
 * left and right corner x values are exchanged (right corner x, left corner
 * x + 0xf). The whole layout is walked twice; the second pass continues
 * after the records of the first.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = first record (an array of 2 * entries
 *     records, at least one entry per pass), a2 = tag (16 bits), a3 = base,
 *     fifth argument (on the caller's stack, at sp + 0x10 of the caller) =
 *     mirror. No return value.
 *   Reads: object.sequence->field_04, the layout there: s16 count at 0, the
 *     id list at 4, the coordinate list at 8 (u16 pairs). The count is
 *     tested after the first entry, so a count of 0 or less still makes one
 *     entry per pass.
 *   Writes: per record, offsets 0x04 to 0x06, 0x0c to 0x0e, 0x14 to 0x16,
 *     0x1c, 0x1d, 0x24 and 0x25; nothing else.
 *   Callee replaced by a recorder (same in both runs): func_8015c09c (one
 *     argument, Sony's library range; no result used). The log copies, at
 *     every call, the first 10 words of the record passed (the part the
 *     callee would initialise) and the whole record array, so that a field
 *     written after a call instead of before it would show.
 *   Aliasing: the object, the sequence step, the layout, the id list, the
 *     coordinate list and the record array are distinct blocks.
 *   Excluded inputs: counts above the room the setup gives the record
 *     array (the setup uses counts up to 6).
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e0ce8_slot0b(Object *obj, Poly28 *buf, u16 tag, int base, int mirror) {
    Slot0bLayout *lay = (Slot0bLayout *)obj->sequence->field_04;
    u8 *ids;
    u16 *coords;
    int pass;
    int n;
    int x;
    int y;
    int near;
    int far;

    for (pass = 0; pass < 2; pass++) {
        n = lay->count;
        ids = lay->ids;
        coords = lay->coords;
        do {
            func_8015c09c(buf);
            buf->field_04 = 0x80;
            buf->field_05 = 0x80;
            buf->field_06 = 0x80;
            buf->field_16 = tag;
            x = *coords++ + 0x60;
            y = *coords++ + 0x50;
            if (mirror == 0) {
                near = x;
                far = x + 0x10;
            } else {
                near = x + 0xf;
                far = x;
            }
            buf->field_0c = near;
            buf->field_1c = near;
            buf->field_0d = y;
            buf->field_14 = far;
            buf->field_15 = y;
            buf->field_1d = y + 0x10;
            buf->field_24 = far;
            buf->field_25 = y + 0x10;
            buf->field_0e = base + (*ids++ << 6);
            buf++;
            n--;
        } while (n > 0);
    }
}
