/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190458;
Object *func_8011f32c(void);

void func_80014dec_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    o->field_45 = 0xff;
    o->field_6b = 0;
    if (o->field_03 >= 2) {
        ref_other.p = func_8011f32c();
        if (ref_other.p != 0) {
            ref_other.p->field_00 = 1;
            ref_other.p->field_02 = 0x1d;
            ref_other.p->field_03 = 0;
            ref_other.p->other = (*(Object **)&data_80190458);
            ref_other.p->field_3c = o;
            obj->field_28 = ref_other.p;
        }
        ref_other.p = func_8011f32c();
        if (ref_other.p != 0) {
            ref_other.p->field_00 = 1;
            ref_other.p->field_02 = 0x1d;
            ref_other.p->field_03 = 1;
            ref_other.p->other = (*(Object **)&data_80190458);
            ref_other.p->field_3c = o;
            obj->field_2c = ref_other.p;
        }
    }
    ref_other.p = o->field_3c;
}
