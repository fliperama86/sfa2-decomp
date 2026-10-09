/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_80031c98_slot28[];
extern Object *data_80051910_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_80017944_slot28(Object *obj, int arg);
void func_80017974_slot28(Object *obj);
void func_800179d4_slot28(void);

void func_80017580_slot28(Object *obj) {
    Object *o;
    HudState *h;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52 += 1;
        func_800179d4_slot28();
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            func_80017974_slot28(o);
            data_80051910_slot28[11] = o;
            o->pos_x = 0x60;
            o->pos_y = 0x20;
            o->field_7a = 0x10;
            o->field_7c = 0x1e0;
            o->field_0d = 0;
            o->field_09 = 4;
            func_80130768(o, 0, data_80031c98_slot28);
        }
        func_80017944_slot28(obj, 1);
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = 1;
            o->field_02 = 0xa6;
            o->field_03 = 3;
            data_80051910_slot28[0] = o;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = 1;
            o->field_02 = 0xa6;
            o->field_03 = 4;
            data_80051910_slot28[1] = o;
        }
        func_80128370();
    }
}
