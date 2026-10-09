/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Exact. The s16 local yi is assigned in both arms and again as the level
   in the later block, so the masked value stays alive next to the u8 copy yb. */
void func_80139304(Object *a, Object *b) {
    Box32 *box;
    u8 index;
    u16 xi;
    s16 yi;
    u8 yb;
    int lvl;

    if (b->field_263 != 0 || b->field_27b != 0 || b->field_163 != 0) {
        return;
    }
    if (a->field_49 == 0 || (b->field_295 == 0 && b->field_28f == 0)) {
        lvl = b->field_69;
        if (a->frame->field_08 < lvl) {
            return;
        }
    }
    index = a->frame->active;
    if (index == 0) {
        return;
    }
    box = &a->wide_boxes[index];
    xi = box->field_12 & 0x7f;
    if (a->field_66 != 0) {
        yi = b->field_265 & 0x7f;
    } else {
        yi = b->field_264 & 0x7f;
    }
    yb = yi;
    if (yb == (u8)xi) {
        return;
    }
    if (a->field_08 != 8) {
        if (a->field_66 != 0) {
            yi = b->field_289;
        } else {
            yi = b->field_288;
        }
        if (box->field_18 < yi) {
            return;
        }
    }
    if (b->frame->box_a != 0) {
        func_8013950c(a, b);
    }
    if (b->frame->box_b != 0) {
        func_801395f8(a, b);
    }
    if (b->frame->box_c != 0) {
        func_801396e4(a, b);
    }
    if (data_80188f30 != 0) {
        b->field_241 = 0;
        func_80139a78(a, b);
        if (a->field_29d == 0) {
            func_8013a3a8(a, b, box);
            func_8013af1c(a, b, box);
            func_80155de0(box, b);
        }
    }
}
