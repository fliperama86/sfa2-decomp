/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80141e5c(Object *object) {
    Box6 *box;
    u16 x;
    u16 ox;
    u8 i;
    ref_other.p = object->other;
    if (object->side == 0) {
        box = boxes_74_left[ref_other.p->kind];
    } else {
        box = boxes_124_right[ref_other.p->kind];
    }
    i = object->frame->field_0b;
    x = ref_other.p->pos_x;
    box += i;
    ox = box->origin;
    if (object->field_0b != 0) ox = -ox;
    object->pos_x = x - ox;
    x = ref_other.p->pos_y;
    object->pos_y = x + box->field_02;
}
