/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void reset_side_table_and_fields(Object *object) {
    u32 *p;
    int z = 0;
    short i;
    game_state.cursor = table_80185cf4;
    if (object->side != 0) {
        game_state.cursor = table_80185e50;
    }
    i = 0x2a;
    do {
        i = i - 1;
        p = game_state.cursor;
        game_state.cursor = p + 1;
        *p = z;
    } while (i != -1);
    object->input_bits = 0xf0fc;
    object->field_180 = z;
    object->frame_count = z;
    object->cursor_timer_b = z;
    object->cursor_timer_a = z;
    object->field_267 = 1;
}
