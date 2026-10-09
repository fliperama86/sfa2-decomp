/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3c0c_slot04_05(Object *obj);

void func_801b3b78_slot04_05(Object *obj, Object *p) {
    obj->field_04++;
    obj->field_1c = p->field_1c;
    obj->field_03 = ((Slot04aObj *)p)->field_a7;
    obj->field_0c = p->field_0c;
    obj->field_0e = p->field_0e;
    obj->field_48 = 0;
    obj->field_0d = p->field_0d;
    obj->field_7a = 0x60;
    obj->field_7c = 0x1e0;
    obj->field_90 = p->field_90;
    obj->field_98 = p->field_98;
    obj->field_9c = p->field_9c;
    func_801b3c0c_slot04_05(obj);
}
