/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801566e8(void *a, Object *object) {
    FrameRecord *r;
    u8 i;
    object->field_b0 = 0x3c;
    object->name_char_3 = 0x20;
    object->phase_index++;
    func_80157090(a, object);
    func_80157174(a, object);
    r = table_8016e5c4;
    i = object->score_rank - 1;
    if (i < 5) {
        r = &r[i];
        r->field_04 = object->name_char_0;
        r->field_05 = object->name_char_1;
        r->handler_index = object->name_char_2;
        r->box_index = object->name_char_3;
    }
    r = table_8016e614;
    i = object->field_121 - 1;
    if (i < 5) {
        r = &r[i];
        r->field_04 = object->name_char_0;
        r->field_05 = object->name_char_1;
        r->handler_index = object->name_char_2;
        r->box_index = object->name_char_3;
    }
}
