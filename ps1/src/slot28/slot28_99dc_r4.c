/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_80036690_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519a4_slot28;
extern ObjectRef data_800519d0_slot28;
void func_8001a958_slot28(Object *obj, int arg);
void func_8001a988_slot28(Object *obj);

void func_80019fb8_slot28(Object *obj) {
    Object *o = data_800519d0_slot28.p;
    if ((s16)o->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52++;
        h->field_60 = 0xb4;
        o = data_8005199c_slot28.p;
        o->field_48 = 0xff;
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            func_8001a988_slot28(o);
            o->pos_x = 0x98;
            o->pos_y = 0x48;
            o->field_7a = 0x60;
            o->field_7c = 0x1e0;
            o->field_0d = 0;
            o->field_09 = 2;
            func_80130768(o, 7, data_80036690_slot28 + 6);
            data_800519a4_slot28.p = o;
        }
        func_8001a958_slot28(obj, 2);
    }
}
