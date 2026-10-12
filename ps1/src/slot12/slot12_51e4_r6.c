/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001569c_slot12(Object *obj) {
    Object *a;
    obj->state_1 = obj->state_1 + 1;
    a = (Object *)func_8011f1e0();
    if (a != 0) {
        a->field_00 = 1;
        a->field_02 = 0xa2;
        a->field_48 = 0;
        a->ot_depth = 1;
        a->pos_x = obj->spawn_x;
        a->pos_y = obj->spawn_y;
        a->field_78 = obj->field_78;
        a->field_03 = obj->field_03;
        a->health = obj->health;
    }
    a = (Object *)func_8011f1e0();
    if (a != 0) {
        a->field_00 = 1;
        a->field_02 = 0xa2;
        a->field_48 = 1;
        a->ot_depth = 2;
        a->pos_x = obj->spawn_x;
        a->pos_y = obj->spawn_y;
        a->field_78 = obj->field_78;
        a->field_03 = obj->field_03;
        a->health = obj->health;
    }
    if (((FrameRecord *)ptr_8019040c)[(s16)obj->health].field_0a == 1) {
        a = (Object *)func_8011f1e0();
        if (a != 0) {
            a->field_00 = 1;
            a->field_02 = 0xa2;
            a->field_48 = 2;
            a->ot_depth = 0;
            a->pos_x = obj->spawn_x;
            a->pos_y = obj->spawn_y;
            a->field_78 = obj->field_78;
            a->field_03 = obj->field_03;
            a->health = obj->health;
        }
    }
}
