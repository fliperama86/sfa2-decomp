/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801566e8(void *a, Object *object) {
    FrameRecord *r;
    u8 i;
    object->field_b0 = 0x3c;
    object->field_11f = 0x20;
    object->field_aa++;
    func_80157090(a, object);
    func_80157174(a, object);
    r = table_8016e5c4;
    i = object->field_120 - 1;
    if (i < 5) {
        r = &r[i];
        r->field_04 = object->field_11c;
        r->field_05 = object->field_11d;
        r->field_06 = object->field_11e;
        r->field_07 = object->field_11f;
    }
    r = table_8016e614;
    i = object->field_121 - 1;
    if (i < 5) {
        r = &r[i];
        r->field_04 = object->field_11c;
        r->field_05 = object->field_11d;
        r->field_06 = object->field_11e;
        r->field_07 = object->field_11f;
    }
}
