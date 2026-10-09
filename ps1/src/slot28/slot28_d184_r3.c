/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001d86c_slot28(Object *obj) {
    Object *p;
    *(int *)&obj->field_10 = *(int *)&obj->field_10 + 0x80000;
    if (obj->pos_x >= 0x98) {
        obj->pos_x = 0x90;
        obj->field_05 = obj->field_05 + 1;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x2e;
            p->field_03 = 2;
            p->field_01 = 1;
            p->field_90 = (void *)0x80060000;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            p->pos_x = 0xb0;
            p->pos_y = 0x40;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            p->box_tables = obj->box_tables;
        }
    }
}
