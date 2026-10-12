/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void spawn_objects_from_table(void) {
    u16 *p = table_8016f658[game_state.kind_index];
    Object *o;
    s16 h;
    while ((h = *p++) >= 0) {
        if (h != 0)
            break;
        o = (Object *)pop_block_from_pool();
        if (o == 0)
            break;
        o->active_count++;
        o->pos_x = *p++;
        o->pos_y = *p++;
        o->ot_depth = *p++;
        o->field_02 = *p++;
        o->table_index = *p++;
        o->facing_side = *p++;
        o->field_0e = *p++;
        o->cycle_value = *p++;
        o->field_1c = *p++;
        o->scroll_factor = *p++;
    }
}
