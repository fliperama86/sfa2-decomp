/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801565f4(void *a, Object *object) {
    u8 first;
    u8 second;

    update_name_entry_input(a, object);
    first = object->score_rank;
    second = object->field_121;
    if (second < first) {
        first = second;
    }
    object->min_rank = first;
    if (object->name_char_count != 4) {
        int t;
        t = object->countdown_timer_b - 1;
        object->countdown_timer_b = t;
        if (t << 16 < 0 || (t = object->countdown_timer - 1, object->countdown_timer = t, t << 16 < 0)) {
            fill_name_chars_with_spaces(a, object);
        } else {
            return;
        }
    }
    store_name_in_score_tables(a, object);
}

void fill_name_chars_with_spaces(void *a, Object *object) {
    s16 i = object->name_char_count;
again:
    ((u8 *)object + i)[0x11c] = 0x20;
    object->name_char_count++;
    i++;
    if (object->name_char_count != 4) {
        goto again;
    }
}
