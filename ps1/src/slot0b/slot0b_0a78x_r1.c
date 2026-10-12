/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0bRot table_801e1fec_slot0b[];

/* What it does (inferred): fills a run of 0x28-byte polygon records with the corners of a list of squares and links each into an ordering-table list. A layout reached through the object's sequence holds a count and a list of coordinate pairs; each pair is mirrored (variant_index), scaled (field_20, y_scale) and rotated (table_index_2 indexes a table of sine and cosine pairs) by the object's state. The records are written from buf, or from buf + count when flag is not 0. */
void func_801e0a78_slot0b(ModObj *obj_arg, void *buf_arg, int unused, int flag) {
    Object *obj = (Object *)obj_arg;
    Poly28 *buf = (Poly28 *)buf_arg;
    Slot0bLayout *lay = (Slot0bLayout *)obj->sequence->data_ptr;
    int n = lay->count;
    u16 *coords = lay->coords;
    Poly28 *p = buf;
    u32 *ot;

    if (flag != 0) {
        p = buf + n;
    }
    ot = (u32 *)data_801987c8;
    do {
        s16 x = *coords++;
        s16 y = *coords++;

        if (obj->variant_index == 0) {
            if (obj->field_20 != 0) {
                x += (x + 8) * (s16)obj->field_20 >> 4;
            }
        } else {
            x = -x - 16;
            if (obj->field_20 != 0) {
                x += (x + 8) * (s16)obj->field_20 >> 4;
            }
        }
        if (obj->y_scale != 0) {
            y += (y + 8) * (s16)obj->y_scale >> 4;
        }
        if (obj->table_index_2 != 0) {
            s16 *t = (s16 *)&table_801e1fec_slot0b[(u8)obj->table_index_2];
            s16 c = *t++;
            s16 s = *t;
            int nx = (s * (s16)(x + 8) >> 8) + (c * (s16)(y + 8) >> 8) - 8;

            y = (-c * (s16)(x + 8) >> 8) + (s * (s16)(y + 8) >> 8) - 8;
            x = nx;
        }
        /* Stand-in, not recovered source: the pair of nested do-while wrappers (the compiler counts each reference inside twice, and the two levels together give the original's order of the loop's values; one level alone changes nothing) reproduces the original's choice of registers. Without it this function differs from the original in 18 instruction slots. */
        do {
            do {
                p->x0 = x + obj->pos_x;
                p->y0 = y + obj->pos_y;
                p->x1 = x + obj->pos_x + 16;
                p->y1 = y + obj->pos_y;
                p->x2 = x + obj->pos_x;
                p->y2 = y + obj->pos_y + 16;
                p->x3 = x + obj->pos_x + 16;
                p->y3 = y + obj->pos_y + 16;
                ((PrimTag *)p)->addr = ((PrimTag *)&ot[obj->ot_depth + 8])->addr;
                ((PrimTag *)&ot[obj->ot_depth + 8])->addr = (u32)p;
                p++;
            } while (0);
        } while (0);
    } while (--n != 0);
}
