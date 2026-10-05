/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Hud *data_8018f5a0;

void func_80150e4c(Object *object) {
    data_8018f5a0->field_48++;
    object->field_c4 = 1;
    object->field_c2 = 0;
    object->field_f0 = 1;
    func_80150e94(object);
}

void func_80150e94(Object *object) {
    int n = object->field_c4 - 1;
    object->field_c4 = n;
    if ((s16)n == 0) {
        object->field_c4 = 1;
        func_80125dc0(4, 0x20, object->field_c2, 0);
        func_80137220(4, 7);
        object->field_c2++;
        if ((s16)object->field_c2 >= 0x20) {
            data_8018f5a0->field_48++;
            object->field_f0 = 0x80;
        }
    }
}
