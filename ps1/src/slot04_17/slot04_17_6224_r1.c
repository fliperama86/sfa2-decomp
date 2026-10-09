/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Object *func_8011f32c(void);

void func_801b6430_slot04_17(Object *obj);
void func_801b65bc_slot04_17(Object *obj);
void func_801b6654_slot04_17(Object *obj);
void func_801b66d0_slot04_17(Object *obj);

void func_801b6224_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *n;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        obj->field_47 = k;
        func_801b66d0_slot04_17(o);
        func_801204f4(ref_other.p->other, ref_other.p->other->side, 0xf);
        func_801b65bc_slot04_17(o);
    } else {
        obj->field_46--;
        if (obj->field_46 == 0 && data_801a4fec != 0) {
            obj->field_46 = 0xc;
            o->field_05++;
            n = func_8011f32c();
            if (n != 0) {
                n->field_00 = 1;
                n->field_02 = 0x28;
                n->field_0e = o->field_0e;
                n->field_48 = 0;
                n->field_3c = o;
                n->field_03 = 0;
                n->field_0b = o->field_0b;
                n->other = o->other;
                o->field_34 = (u32)n;
                n->field_76 = 0x240;
                n->field_7a = 0x60;
                n->field_7c = 0x1e2;
                n->field_98 = data_80172a48;
                n->field_78 = 0;
                n->field_90 = (void *)0x800fb100;
                n->field_9c = data_80173c9c;
                n->field_66 = o->field_66;
            }
        }
    }
}

void func_801b6370_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        o->field_05 = 0;
        obj->field_47 = k;
        func_801b66d0_slot04_17(o);
        func_801204f4(ref_other.p->other, ref_other.p->other->side, 0xf);
        func_801b65bc_slot04_17(o);
    } else {
        obj->field_46--;
        if (obj->field_46 == 0) {
            o->field_05++;
            func_801b6654_slot04_17(o);
        } else {
            func_801b6430_slot04_17(o);
        }
    }
}
