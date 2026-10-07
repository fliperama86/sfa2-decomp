/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_800797fc_slot00[];
extern ObjectFn data_8007980c_slot00[];

void func_80076e54_slot00(Object *o) {
    o->field_46 = (s16)o->field_46 - 1;
    if ((s16)o->field_46 < 0) {
        o->field_00 = 1;
        o->field_04 = 1;
        o->field_06 = 1;
        o->field_05 = 0;
        o->field_07 = 0;
        o->field_4c = -o->field_4c;
        if (o->field_ad != 0) {
            o->field_50 = data_800797fc_slot00[3];
        } else {
            o->field_50 = data_800797fc_slot00[o->field_ac >> 1];
        }
    }
    func_80131094(o);
}

void func_80076ef0_slot00(Object *o) {
    data_8007980c_slot00[o->field_05](o);
    func_8011ffdc(o);
}
