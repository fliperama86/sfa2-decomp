/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011ffdc(Object *obj);

void func_800139e4_slot01(Object *obj) {
    Object *src = (Object *)obj->field_28;
    int k = obj->field_03 - 1;
    obj->pos_x = src->pos_x + (k << 8);
    obj->pos_y = src->pos_y;
    func_8011ffdc(obj);
}
