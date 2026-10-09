/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4db8_slot04_0b(Object *obj, int index);
void func_801b4e0c_slot04_0b(Object *obj, int dx, int dy);

void func_801b47b8_slot04_0b(Object *obj) {
    obj->field_0d = 0x10;
    obj->field_05++;
    func_801b4e0c_slot04_0b(obj, 6, 0);
    func_801b4db8_slot04_0b(obj, 0x16);
    func_8011ffdc(obj);
}

void func_801b4810_slot04_0b(Object *obj) {
    Object *n;
    if (ref_other.p->field_3a & 1) {
        obj->field_05++;
        func_801b4e0c_slot04_0b(obj, -0x19, -3);
        func_801b4db8_slot04_0b(obj, 0x17);
        n = (Object *)func_8011f1e0();
        if (n != 0) {
            n->field_00 = 1;
            n->field_02 = 6;
            n->field_03 = 2;
            n->field_3c = obj->field_3c;
            n->field_66 = ((Slot04aObj *)ref_other.p)->field_a6;
            n->field_08 = 0x20;
            n->field_90 = ref_other.p->field_90;
            n->field_98 = ref_other.p->field_98;
            n->field_9c = ref_other.p->field_9c;
            if (n->field_66 == 0) {
                n->field_7a = 0x100;
            } else {
                n->field_7a = 0x120;
            }
            n->field_7c = 0x1e0;
        }
    }
    func_8011ffdc(obj);
}
