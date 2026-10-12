/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void set_position_from_linked_object_offset_slot01(Object *obj) {
    Object *src = (Object *)obj->linked_object;
    int k = obj->table_index - 1;
    obj->pos_x = src->pos_x + (k << 8);
    obj->pos_y = src->pos_y;
    clear_or_set_flag_by_pos_x_limit(obj);
}
