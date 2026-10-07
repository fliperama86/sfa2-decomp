/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e78e0_slot0f(Object *obj);
Block172 *func_8011f1e0(void);

void func_800e7748_slot0f(Object *obj) {
    Object *p;
    FrameRecord *fr;
    u8 t = obj->field_05;
    obj->field_05 = t + 1;
    func_800e78e0_slot0f(obj);
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x97;
        p->field_48 = 0;
        p->field_09 = 1;
        p->pos_x = obj->field_7a;
        p->pos_y = obj->field_7c;
        p->field_03 = obj->field_03;
        p->field_5c = obj->field_5c;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x97;
        p->field_48 = 1;
        p->field_09 = 2;
        p->pos_x = obj->field_7a;
        p->pos_y = obj->field_7c;
        p->field_03 = obj->field_03;
        p->field_5c = obj->field_5c;
    }
    fr = (FrameRecord *)ptr_8019040c;
    if (fr[obj->field_03].field_0a != 0) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x97;
            p->field_48 = 2;
            p->field_09 = 0;
            p->pos_x = obj->field_7a;
            p->pos_y = obj->field_7c;
            p->field_03 = obj->field_03;
            p->field_5c = obj->field_5c;
        }
    }
}
