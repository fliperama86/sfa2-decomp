/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern Quad data_80185c04[][8];
Tri *get_log_record_at_offset(Object *object, int offset);
void func_8015bf70(u8 *a, u32 *b, u32 *c);

/*
 * What it does (inferred, not an original name): lays out the 16-byte
 * records of a sprite strip. A sequence entry picks, by the object's field_02
 * and a frame distance, a size value, a stream header, a flag and a mode byte
 * from four tables. The header gives a count and the offset of a stream of
 * halfwords. Each group of the stream gives an x step and a y step (subtracted
 * when mode bit 0, respectively bit 1, is set, else added) and a record count
 * minus one; each record gets an x and a y halfword and x moves by 16 per
 * record, back by 16 after each group. A mode byte of 4 or more stores nothing
 * but still counts the records. At the end it calls func_8015bf70 (AddPrims)
 * with a list slot and two words of a table, which links the strip into the
 * draw list.
 */
void fill_cell_record_rects_from_history(Object *obj, u16 column, u16 frame, Block172 *second) {
    SeqHeader *r;
    SeqHeader *hdr;
    Pooled *rec;
    u16 *sp;
    u16 idx, dist, size, count, group; /* count as unsigned int differs in 29 instruction lines */
    unsigned int i, j;
    int yy;
    s16 x0, x1, y0, y1;
    int mode, fa, fb;

    idx = obj->field_02;
    r = (SeqHeader *) get_log_record_at_offset(obj, frame);
    dist = data_801900f8[idx] - r->count + 1;
    if (dist >= 10)
        return;
    size = data_80183d9c[idx][dist];
    if (size == 0)
        return;
    hdr = (SeqHeader *) data_80183dec[idx][dist];
    if (hdr == 0)
        return;
    count = hdr->count;
    if (count == 0)
        return;
    if (data_80183dc4[idx][dist] == 0) {
        if (size != data_80183d9c[idx][dist - 1])
            return;
        if ((u32) hdr != data_80183dec[idx][dist - 1])
            return;
    }
    x0 = r->pos_x - 0x10 - box_margin[0];
    x1 = r->pos_x - box_margin[0];
    yy = r->pos_y + (data_801aa5ea[0] - 8);
    /* The original loads only the low byte of the entry (lbu): without the (u8) the build loads 16 bits and differs in 1 instruction line. */
    mode = (u8) data_80183d74[idx][dist];
    y0 = yy;
    y1 = yy;
    if (obj->field_0f != 0) {
        y0 = game_state.scroll_y + yy;
        y1 = y0;
    }
    rec = (Pooled *) &data_801987cc[data_801a27d0][0x280 + obj->side * 320 + column * 80];
    sp = (u16 *) ((u8 *) obj->data_ptr_a + (hdr->pos_x & 0xfffe));
    for (i = 0; i < count; i++) {
        /* fa and fb as locals; the plain tests inside the ifs differ in 14 instruction lines. */
        fa = mode & 1;
        fb = mode & 2;
        /* One u16 v declared at the top of the loop body, or at function level, differs in 6 instruction lines. */
        if (fa) {
            u16 v = *sp++;
            x0 = x0 - v;
            x1 = x1 - v;
        } else {
            u16 v = *sp++;
            x0 = x0 + v;
            x1 = x1 + v;
        }
        if (fb) {
            u16 v = *sp++;
            y1 = y1 - v;
            y0 = y0 - v;
        } else {
            u16 v = *sp++;
            y1 = y1 + v;
            y0 = y0 + v;
        }
        group = *sp++ + 1;
        /* The outer counter counts records: it is stepped back by one here and forward once per record below. */
        i--;
        for (j = 0; j < group; rec++, j++) {
            i++;
            /* The plain switch (mode) differs in 64 instruction lines and saves one more register (s4). */
            switch (mode & 0xff) {
            case 0:
                x0 += 0x10;
                rec->rect_x = x1;
                x1 += 0x10;
                rec->rect_y = y1;
                break;
            case 1:
                rec->rect_x = x0;
                x0 -= 0x10;
                x1 -= 0x10;
                rec->rect_y = y1;
                break;
            case 2:
                x0 += 0x10;
                rec->rect_x = x1;
                x1 += 0x10;
                rec->rect_y = y0;
                break;
            case 3:
                rec->rect_x = x0;
                x0 -= 0x10;
                x1 -= 0x10;
                rec->rect_y = y0;
                break;
            }
        }
        if (fa) {
            x0 += 0x10;
            x1 += 0x10;
        } else {
            x0 -= 0x10;
            x1 -= 0x10;
        }
    }
    /* The two words of one entry of data_80185c04, taken as words of its row with a 16-bit index. The entry's own address, (u32 *) &data_80185c04[sel][side * 4 + column] and that + 1, differs in 28 instruction lines; the word index without the (u16) in 20. */
    func_8015bf70((u8 *) data_801987c8 + (second->field_09 * 4 + 0x30),
                  &((u32 *) data_80185c04[data_801a27d0])[(u16) ((obj->side * 4 + column) * 2)],
                  &((u32 *) data_80185c04[data_801a27d0])[(u16) ((obj->side * 4 + column) * 2) + 1]);
}
