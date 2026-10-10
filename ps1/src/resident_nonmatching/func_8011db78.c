/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling,
 * register choice and the shape of the loop. The exact owner of the bytes in
 * the PS1 build stays the raw bytes of the resident image; the build does
 * not use this file. The differential test next to it (difftest.py with
 * func_8011db78.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): lays out the 16-byte
 * records of a sprite strip. The object's sequence entry for the frame
 * (found through func_801250c0) picks, by the object's field_02 and a frame
 * distance d, an entry of four tables: a size/offset value, a stream header,
 * a flag and a mode byte. The header gives a count and the offset of a
 * stream of halfwords in the object's data (field_98). For each group of
 * the stream the code reads an x step and a y step (subtracted when mode
 * bit 0, respectively bit 1, is set, else added), then a record count
 * minus one, and writes for each record an x and a y halfword (offsets 8
 * and 0xa) into a record buffer; x moves by 16 per record, downwards when
 * mode bit 0 is set, and moves back by 16 after each group. A mode byte
 * of 4 or more stores nothing but still counts the records. At the end
 * it calls func_8015bf70 (AddPrims) with a list slot and two words of a
 * table, which links the strip into the draw list.
 *
 * Contract (what the code reads and writes; roles are inferred):
 *   Arguments: a0 = object, a1 = column (16 bits), a2 = frame number
 *     (16 bits), a3 = a second object of which only field_09 is read.
 *     Upper halves of a1 and a2 are zero. No return value.
 *   Callees: func_801250c0(object, frame) is replaced by a recorder that
 *     returns a block the setup made (fields 0, 8 and 0xa are read: a
 *     frame number, and two position halfwords); func_8015bf70 (library,
 *     3 arguments) is replaced by a recorder that returns nothing used.
 *   Reads: object field_02, field_0f, side (0xa6), field_98 (a pointer);
 *     data_801900f8[field_02]; the tables data_80183d9c, data_80183dc4,
 *     data_80183dec and data_80183d74 at [field_02][d] with d from 0 to 9,
 *     and at [field_02][d - 1] when the flag table entry is 0; box_margin;
 *     data_801aa5ea; game_state.field_92 when field_0f is not 0;
 *     data_801a27d0 (buffer selector); data_801987c8; the stream.
 *   Returns early, writing nothing but the recorders' log, in these
 *     cases: d is 10 or more; the size entry is 0; the header pointer is 0;
 *     the header count is 0; the flag entry is 0 and the size entry or the
 *     header pointer differs from the one before. The early returns before
 *     the strip is laid out make no call to func_8015bf70.
 *   Writes: records at data_801987cc + (side * 320 + column * 80 + 640)
 *     * 16 + selector * 0x5000, 16 bytes each, halfwords at 8 and 0xa.
 *   Watched at every recorded call (copied into the log): the object (0x394
 *     bytes), the second object (0xac bytes) and the 320 bytes of the record
 *     buffer from its first record. No argument of a recorded call points
 *     at memory the function fills (the list slot and table words are not
 *     written by it), so there are no pointees.
 *   Aliasing: the object, the second object, the call block, the header
 *     and the stream are distinct blocks of the setup. The tables overlap
 *     one another in the image (the setup writes only the cells the case
 *     reads, and no two of those cells are the same bytes).
 *   Excluded inputs: a stream value that makes a group of 65535 records
 *     (the original handles it but the case would take a million steps);
 *     a field_02 above 40, so that the table cells stay clear of the
 *     fields of game_state that the function reads.
 *   Simplification: the original keeps two running x values (16 apart) and
 *     two running y values and stores one of each by mode (0 and 2: the
 *     upper x, 1 and 3: the lower x; 0 and 1: one y, 2 and 3: the other).
 *     The two y values are always equal, and the stored x of each mode
 *     moves the same way in the original and here, so the C keeps one x
 *     and one y (inferred from the listing; the test is the check).
 *   Not reached by any input: two instruction slots of the original, at
 *     offsets 0x298 and 0x29c: the jump after a "mode is 0" test that has
 *     just failed for a mode already known to be below 2 and not 1.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Quad data_80185c04[][8];
Tri *func_801250c0(Object *object, int offset);
void func_8015bf70(u8 *a, u32 *b, u32 *c);

void func_8011db78(void *a, int b, int c, Block172 *d) {
    Object *o = a;
    u16 column = b;
    u16 frame = c;
    SeqHeader *r;
    SeqHeader *hdr;
    Pooled *rec;
    u16 *sp;
    u16 idx, dist, size, count, group;
    u16 i, j;
    int mode, step;
    int x, y, v;
    int fb;

    idx = o->field_02;
    r = (SeqHeader *)func_801250c0(o, frame & 0xffff);
    dist = data_801900f8[idx] - r->field_00 + 1;
    if (dist >= 10)
        return;
    size = data_80183d9c[idx][dist];
    if (size == 0)
        return;
    hdr = (SeqHeader *)data_80183dec[idx][dist];
    if (hdr == 0)
        return;
    count = hdr->field_00;
    if (count == 0)
        return;
    if (data_80183dc4[idx][dist] == 0) {
        if (size != data_80183d9c[idx][dist - 1])
            return;
        if ((u32)hdr != data_80183dec[idx][dist - 1])
            return;
    }
    y = r->field_0a - 8 + data_801aa5ea[0];
    if (o->field_0f != 0)
        y = game_state.field_92 + y;
    mode = data_80183d74[idx][dist] & 0xff;
    fb = mode & 2;
    step = (mode & 1) ? -0x10 : 0x10;
    x = r->field_08 - box_margin[0];
    if (mode & 1)
        x -= 0x10;
    rec = (Pooled *)(data_801987cc + (o->side * 320 + column * 80 + 640) * 16 + data_801a27d0 * 0x5000);
    sp = (u16 *)((u8 *)o->field_98 + (hdr->field_08 & 0xfffe));
    for (i = 0; i < count; ) {
        v = *sp++;
        if (mode & 1)
            x -= v;
        else
            x += v;
        v = *sp++;
        if (fb)
            y -= v;
        else
            y += v;
        group = *sp++ + 1;
        for (j = 0; j < group; j++, rec++) {
            if (mode < 4) {
                rec->field_08 = x;
                rec->field_0a = y;
                x += step;
            }
        }
        x -= step;
        i += group;
    }
    func_8015bf70((u8 *)data_801987c8 + (d->field_09 * 4 + 0x30),
                  &((u32 *)data_80185c04)[(u16)((o->side * 4 + column) * 2) + data_801a27d0 * 16],
                  &((u32 *)data_80185c04)[(u16)((o->side * 4 + column) * 2) + data_801a27d0 * 16 + 1]);
}
