/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;

void func_801e1554_slot0b(Slot0bObj *obj);
void func_801e1624_slot0b(Slot0bObj *obj);

void func_801e131c_slot0b(Slot0bObj *obj) {
    int v;
    if (data_80190468.p->field_ab == 1 || (v = obj->field_46 - 1, obj->field_46 = v, (s16)v < 0)) {
        obj->field_05++;
        if (obj->field_03 == 0) {
            func_80120554(0, 0, 0x32d);
        }
        ((Object *)obj)->pos_x = obj->field_5c;
        ((Object *)obj)->pos_y = obj->field_5e;
        func_801e1624_slot0b(obj);
    } else {
        func_801e1554_slot0b(obj);
    }
}
