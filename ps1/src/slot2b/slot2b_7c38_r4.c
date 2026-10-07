/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8007ef24_slot2b;
void func_8011f14c(Slab172 *o);

void func_80077fcc_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->field_14 -= s->field_50;
    s->field_50 += s->field_58;
    if (data_8007ef24_slot2b <= o->pos_y) {
        o->field_05++;
        o->field_14 = 0;
        o->field_50 = 0x20000;
        o->field_58 = -0x4000;
        o->pos_y = data_8007ef24_slot2b;
    }
    func_80131094(o);
}

void func_80078050_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->field_14 -= s->field_50;
    s->field_50 += s->field_58;
    if (data_8007ef24_slot2b <= o->pos_y) {
        o->field_04 = 3;
        o->field_05 = 0;
        o->field_06 = 0;
        o->field_07 = 0;
        o->field_14 = 0;
    }
    func_80131094(o);
}

void func_800780c0_slot2b(Object *o) {
    Object *p = o->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}
