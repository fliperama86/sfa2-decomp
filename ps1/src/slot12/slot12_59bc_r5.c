/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015fd4_slot12(Object *obj, int a) {
    Object *n;

    if (((FrameRecord *)ptr_8019040c)[(s16)obj->health].field_0a != 0) {
        n = (Object *)func_8011f1e0();
        if (n != 0) {
            n->field_00 = 1;
            n->field_02 = 0xa2;
            n->field_48 = 2;
            n->ot_depth = 0;
            n->pos_x = obj->spawn_x;
            n->pos_y = obj->spawn_y;
            n->field_03 = 0;
            n->health = obj->health;
        }
    }
}
