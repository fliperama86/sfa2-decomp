/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80023d9c_slot01[];
Object *func_8011f32c(void);
void func_8001257c_slot01(void);

void func_800124f4_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *s = o->field_3c;
    Object *p = func_8011f32c();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x23;
        p->field_03 = 0;
        p->field_48 = s->side;
        p->field_66 = 0;
        p->field_58 = (s32)data_80023d9c_slot01;
        p->field_4c = 0;
        p->field_50 = 0;
        p->field_0b = 0;
        p->field_3c = o;
        obj->field_30 = p;
    }
    func_8001257c_slot01();
}
