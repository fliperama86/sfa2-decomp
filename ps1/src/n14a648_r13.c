/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_80150f40(Object *object) {
    if (object->field_225 == 0) {
        data_8018f5a0->field_48++;
        object->field_c2 = 0x1f;
    }
}
