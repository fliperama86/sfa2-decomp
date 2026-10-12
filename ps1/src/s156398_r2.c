/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801565f4(void *a, Object *object) {
    u8 first;
    u8 second;

    update_name_entry_input(a, object);
    first = object->field_120;
    second = object->field_121;
    if (second < first) {
        first = second;
    }
    object->field_122 = first;
    if (object->field_123 != 4) {
        int t;
        t = object->field_b4 - 1;
        object->field_b4 = t;
        if (t << 16 < 0 || (t = object->field_b0 - 1, object->field_b0 = t, t << 16 < 0)) {
            fill_name_chars_with_spaces(a, object);
        } else {
            return;
        }
    }
    func_801566e8(a, object);
}

void fill_name_chars_with_spaces(void *a, Object *object) {
    s16 i = object->field_123;
again:
    ((u8 *)object + i)[0x11c] = 0x20;
    object->field_123++;
    i++;
    if (object->field_123 != 4) {
        goto again;
    }
}
