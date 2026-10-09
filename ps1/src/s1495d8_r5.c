/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80149f20(Object *object, Object *unused);

u8 func_80149e3c(Object *a, Object *b) {
    if (a->field_45 != 0 && b->field_45 == 0) return 0;
    if (b->field_225 != 0) return 1;
    if (b->frame->field_0c & 0x80) return 0;
    if (b->field_159 == 0) return 0;
    return (u8)func_80149f20(a, b) != 0;
}

u8 func_80149ec8(Object *a, Object *b, s16 *c, int d) {
    if ((u16)((u16)b->pos_x - (u16)a->pos_x + 0x80) <= 0x100) {
        return func_80149f14(b) != 0;
    } else {
        return 0;
    }
}
