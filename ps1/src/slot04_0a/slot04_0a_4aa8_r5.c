/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_801b5154_slot04_0a(Object *obj);

void func_801b5048_slot04_0a(Object *obj) {
    Object *p;
    SequenceStep **t;
    obj->field_04++;
    p = obj->field_3c;
    obj->pos_x = p->pos_x;
    *(u16 *)&obj->pos_y = ((Slot04aObj *)p)->field_70;
    obj->field_1c = p->field_1c;
    obj->field_0e = p->field_0e;
    obj->field_0c = p->field_0c;
    obj->field_0d = p->field_0d;
    obj->field_0b = p->field_0b;
    if (p->side == 0) {
        t = data_1f8000b4;
    } else {
        t = data_1f800164;
    }
    func_80130768(obj, 0x15, t);
}

void func_801b50f4_slot04_0a(Object *obj) {
    if (obj->field_3c->field_06 >= 2) {
        func_801b5154_slot04_0a(obj);
    } else {
        func_80131094(obj);
        func_8011ffdc(obj);
    }
}
