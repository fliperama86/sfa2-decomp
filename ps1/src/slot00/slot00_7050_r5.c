/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800776a8_slot00(Object *obj, short delta);

void func_800774f0_slot00(Object *obj) {
    Object *p = obj->field_3c;
    Object *n;
    Object *q;
    if (p->field_a0 != 7) {
        p->field_a0 = p->field_a0 + 1;
    }
    obj->field_04 = obj->field_04 + 1;
    func_800776a8_slot00(obj, 0x12);
    q = ((Slot00Obj *)obj)->field_a8;
    n = (Object *)func_8011f1e0();
    if (n != 0) {
        n->field_00 = 1;
        n->field_03 = 6;
        ((Slot00Obj *)n)->field_10 = ((Slot00Obj *)q)->field_10;
        ((Slot00Obj *)n)->field_14 = ((Slot00Obj *)q)->field_14;
        n->field_0b = p->field_0b;
        n->field_0e = p->field_0e;
        n->field_0c = p->field_0c;
        n->field_0d = p->field_0d;
        n->field_1c = p->field_1c;
        n->field_1e = p->field_1e;
        n->field_02 = 0xc;
        n->field_08 = 0x20;
        n->field_90 = p->field_90;
        n->field_98 = p->field_98;
        n->field_9c = p->field_9c;
        n->field_7a = p->field_7a;
        n->field_7c = p->field_7c;
        n->field_66 = p->side;
    }
    func_801204f4(p, p->side, 0x15);
}
