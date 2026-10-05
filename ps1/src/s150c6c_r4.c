/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Hud *data_8018f5a0;

void func_80150f7c(Object *object) {
    int n = object->field_c4 - 1;
    object->field_c4 = n;
    if ((s16)n == 0) {
        int m;
        object->field_c4 = 1;
        func_80125dc0(4, 0x20, object->field_c2, 0);
        func_80137220(4, 7);
        m = object->field_c2 - 1;
        object->field_c2 = m;
        if ((s16)m < 0) {
            data_8018f5a0->field_48++;
        }
    }
}
