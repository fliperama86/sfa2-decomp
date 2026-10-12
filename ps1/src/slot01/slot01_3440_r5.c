/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800139e4_slot01(Object *obj) {
    Object *src = (Object *)obj->linked_object;
    int k = obj->table_index - 1;
    obj->pos_x = src->pos_x + (k << 8);
    obj->pos_y = src->pos_y;
    func_8011ffdc(obj);
}
