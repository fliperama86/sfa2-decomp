/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80139d4c(Object *a0, Object *a1, Box32 *unused) {
    if (game_state.field_30) {
        if (a1->field_1a0) {
call:
            func_8013a050(a0, a1);
        }
    } else {
        if (a1->field_d8 != 0) goto call;
        func_8013a154(a0, a1, &a0->wide_boxes[a0->frame->active]);
    }
}
