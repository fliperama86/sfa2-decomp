/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c4f08_slot04_12[];

void func_801b417c_slot04_12(Object *obj, Object *p);

void func_801b417c_slot04_12(Object *obj, Object *p) {
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
            func_80130700(obj, data_801c4f08_slot04_12[2 + p->side][a]);
        }
    }
}
