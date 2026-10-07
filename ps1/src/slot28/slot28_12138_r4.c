/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051ba4_slot28[];
extern Object *data_80051bd0_slot28[];
extern ObjectRef data_80051bdc_slot28;
extern SequenceStep *data_80047bdc_slot28[];
extern SequenceStep *data_80047be4_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_80022ccc_slot28(void);
void func_80022c6c_slot28(Object *obj);
void func_80022c3c_slot28(Object *obj, int arg);

void func_800226c4_slot28(Object *obj) {
    Object *o;
    if (data_80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_80022ccc_slot28();
        o = data_80051bd0_slot28[0];
        o->field_01 = 1;
        func_80130768(o, 0, data_80047bdc_slot28);
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            func_80022c6c_slot28(o);
            o->pos_x = 0xe8;
            o->pos_y = 0xa0;
            o->field_7a = 0x60;
            o->field_7c = 0x1e0;
            o->field_0d = 0;
            o->field_09 = 2;
            func_80130768(o, 4, data_80047be4_slot28);
            data_80051ba4_slot28[0] = o;
            o->field_09 = 3;
        }
        func_80128370();
        func_80022c3c_slot28(obj, 2);
    }
}

void func_800227d0_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051bdc_slot28.p)->field_3a < 0) {
        data_8018f5a0->field_60 = 0xc0;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
    }
}
