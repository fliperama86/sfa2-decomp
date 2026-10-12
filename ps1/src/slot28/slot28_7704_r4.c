/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80017ed0_slot28(Object *obj, Object *src);

void func_80017c50_slot28(Object *obj) {
    Object *p;
    *(u32 *)&obj->pos_y_frac += 0x8000;
    if (obj->pos_y >= 0xa1) {
        obj->pos_y = 0xa0;
        obj->state_1++;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x34;
            p->pos_x = 0xf0;
            p->pos_y = 0x80;
            p->spawn_x = 0x60;
            p->field_03 = 1;
            p->spawn_y = 0x1e0;
            p->field_0d = 0;
            p->ot_depth = 1;
            p->field_01 = 1;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            p->box_tables = obj->box_tables;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80017ed0_slot28(p, obj);
            p->pos_x = 0xf0;
            p->pos_y = 0x80;
            p->ot_depth = 2;
            func_80130768(p, 2, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80017ed0_slot28(p, obj);
            p->pos_x = 0xe0;
            p->pos_y = 0x50;
            p->ot_depth = 3;
            func_80130768(p, 4, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80017ed0_slot28(p, obj);
            p->pos_x = 0xf0;
            p->pos_y = 0x90;
            p->ot_depth = 3;
            func_80130768(p, 5, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80017ed0_slot28(p, obj);
            p->pos_x = 0x70;
            p->pos_y = 0x50;
            p->ot_depth = 3;
            func_80130768(p, 6, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80017ed0_slot28(p, obj);
            p->pos_x = 0x70;
            p->pos_y = 0x80;
            p->ot_depth = 3;
            func_80130768(p, 7, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xa6;
            p->field_03 = 6;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xa6;
            p->field_03 = 7;
        }
    }
}
