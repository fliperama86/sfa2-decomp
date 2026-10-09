/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8002616c_slot28(Object *obj) {
    Object *b;
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + 0x60000;
    if (obj->pos_x >= 0x88) {
        obj->field_05 = obj->field_05 + 1;
        b = obj->field_3c;
        b->field_48 = 0xff;
        func_80130768(obj, 2, (SequenceStep **)obj->box_tables);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x2d;
            b->field_03 = 2;
            b->field_01 = 1;
            b->field_90 = (void *)0x80060000;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            b->pos_x = 0xb0;
            b->pos_y = 0x70;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_09 = 2;
            b->box_tables = (BoxTables *)obj->field_58;
        }
    }
}
