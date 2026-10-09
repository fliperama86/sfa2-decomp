/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c4670_slot04_04[];

void func_801b40ec_slot04_04(Object *obj, Object *p) {
    u8 a;

    a = p->frame->field_09;
    if (a == 0) {
        obj->field_48 = 0;
    } else {
        obj->pos_x = p->pos_x;
        obj->pos_y = p->pos_y;
        obj->field_0b = p->field_0b;
        obj->field_01 = 1;
        if (obj->field_48 == a) {
            func_80131094(obj);
        } else {
            obj->field_48 = a;
            func_80130700(obj, data_801c4670_slot04_04[p->side][a]);
        }
    }
}

void func_801b4198_slot04_04(Object *obj, Object *p) {
    u8 a;

    a = p->frame->field_0f;
    if (a == 0) {
        obj->field_48 = 0;
    } else {
        obj->pos_x = p->pos_x;
        obj->pos_y = p->pos_y;
        obj->field_0b = p->field_0b;
        obj->field_01 = 1;
        if (obj->field_48 == a) {
            func_80131094(obj);
        } else {
            obj->field_48 = a;
            func_80130700(obj, data_801c4670_slot04_04[2 + p->side][a]);
        }
    }
}
