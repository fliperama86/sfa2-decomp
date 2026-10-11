/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0bRot table_801e1fec_slot0b[];

/* What it does (inferred): fills a run of 0x28-byte polygon records with the corners of a list of squares and links each into an ordering-table list. A layout reached through the object's sequence holds a count and a list of coordinate pairs; each pair is mirrored (field_48), scaled (field_20, field_22) and rotated (field_24 indexes a table of sine and cosine pairs) by the object's state. The records are written from buf, or from buf + count when flag is not 0. */
void func_801e0a78_slot0b(ModObj *obj_arg, void *buf_arg, int unused, int flag) {
    Object *obj = (Object *)obj_arg;
    Poly28 *buf = (Poly28 *)buf_arg;
    Slot0bLayout *lay = (Slot0bLayout *)obj->sequence->field_04;
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

        if (obj->field_48 == 0) {
            if (obj->field_20 != 0) {
                x += (x + 8) * (s16)obj->field_20 >> 4;
            }
        } else {
            x = -x - 16;
            if (obj->field_20 != 0) {
                x += (x + 8) * (s16)obj->field_20 >> 4;
            }
        }
        if (obj->field_22 != 0) {
            y += (y + 8) * (s16)obj->field_22 >> 4;
        }
        if (obj->field_24 != 0) {
            s16 *t = (s16 *)&table_801e1fec_slot0b[(u8)obj->field_24];
            s16 c = *t++;
            s16 s = *t;
            int nx = (s * (s16)(x + 8) >> 8) + (c * (s16)(y + 8) >> 8) - 8;

            y = (-c * (s16)(x + 8) >> 8) + (s * (s16)(y + 8) >> 8) - 8;
            x = nx;
        }
        /* Exception, not recovered source: the group of whole statements below stands in two nested do-while wrappers. A wrapper makes the compiler count each reference inside it twice, which gives the original's registers for the loop's values; the second, nested wrapper is not one of the two uses the guide accepts. It adds no instruction. Without the nested one this function differs from the original in 18 instruction lines. */
        do {
            do {
                p->field_08 = x + obj->pos_x;
                p->field_0a = y + obj->pos_y;
                p->field_10 = x + obj->pos_x + 16;
                p->field_12 = y + obj->pos_y;
                p->field_18 = x + obj->pos_x;
                p->field_1a = y + obj->pos_y + 16;
                p->field_20 = x + obj->pos_x + 16;
                p->field_22 = y + obj->pos_y + 16;
                ((PrimTag *)p)->addr = ((PrimTag *)&ot[obj->field_09 + 8])->addr;
                ((PrimTag *)&ot[obj->field_09 + 8])->addr = (u32)p;
                p++;
            } while (0);
        } while (0);
    } while (--n != 0);
}
