/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015fd4_slot12(Object *obj, int a) {
    Object *n;

    if (((FrameRecord *)ptr_8019040c)[(s16)obj->health].offset_index != 0) {
        n = (Object *)func_8011f1e0();
        if (n != 0) {
            n->active_count = 1;
            n->field_02 = 0xa2;
            n->variant_index = 2;
            n->ot_depth = 0;
            n->pos_x = obj->spawn_x;
            n->pos_y = obj->spawn_y;
            n->table_index = 0;
            n->health = obj->health;
        }
    }
}
