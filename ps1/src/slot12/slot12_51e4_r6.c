/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001569c_slot12(Object *obj) {
    Object *a;
    obj->field_05 = obj->field_05 + 1;
    a = (Object *)func_8011f1e0();
    if (a != 0) {
        a->field_00 = 1;
        a->field_02 = 0xa2;
        a->field_48 = 0;
        a->field_09 = 1;
        a->pos_x = obj->field_7a;
        a->pos_y = obj->field_7c;
        a->field_78 = obj->field_78;
        a->field_03 = obj->field_03;
        a->field_5c = obj->field_5c;
    }
    a = (Object *)func_8011f1e0();
    if (a != 0) {
        a->field_00 = 1;
        a->field_02 = 0xa2;
        a->field_48 = 1;
        a->field_09 = 2;
        a->pos_x = obj->field_7a;
        a->pos_y = obj->field_7c;
        a->field_78 = obj->field_78;
        a->field_03 = obj->field_03;
        a->field_5c = obj->field_5c;
    }
    if (((FrameRecord *)ptr_8019040c)[(s16)obj->field_5c].field_0a == 1) {
        a = (Object *)func_8011f1e0();
        if (a != 0) {
            a->field_00 = 1;
            a->field_02 = 0xa2;
            a->field_48 = 2;
            a->field_09 = 0;
            a->pos_x = obj->field_7a;
            a->pos_y = obj->field_7c;
            a->field_78 = obj->field_78;
            a->field_03 = obj->field_03;
            a->field_5c = obj->field_5c;
        }
    }
}
