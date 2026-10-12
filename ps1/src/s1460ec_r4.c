/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u16 get_table_value_and_count_hit(Object *object) {
    u16 result = 0xf;
    int kind;
    if (object->field_167 != 0xb) {
        kind = object->field_255;
        if (kind == 6) {
            result = table_8017cae4[4 + (object->saved_meter >> 4)];
        } else {
            result = table_8017cae4[(kind & 0xfe) >> 1];
        }
        if (game_state.mode != 3) {
            object->hit_count++;
            if (object->field_255 == 6) {
                object->counter_ed++;
            }
        }
    }
    return result;
}

void call_state_0_handler_from_cb00_table(Block172 *block) {
    Object *object = (Object *)block;
    fn_table_8017cb00[object->state_0](object);
}
