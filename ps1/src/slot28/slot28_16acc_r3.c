/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ccc_slot28[];
extern Object *data_80051cd0_slot28[];
extern ObjectRef data_80051cdc_slot28;
extern SequenceStep *data_800502c8_slot28[];
extern SequenceStep *data_800502d4_slot28[];
void func_800272ec_slot28(void);
void func_8002725c_slot28(Object *o, int arg);

/* data_80051ccc_slot28[1] is the word that other units name data_80051cd0_slot28. */
void func_80026e10_slot28(Object *obj) {
    HudState *h;
    Object *o;
    Object *o2;
    int v;
    if (obj->field_f0 == 0) {
        v = 0x1e0;
        h = data_8018f5a0;
        h->field_60 = v;
        h->field_52++;
        func_800272ec_slot28();
        func_80130768(data_80051ccc_slot28[0], 0, data_800502c8_slot28);
        o = data_80051cd0_slot28[0];
        o->pos_x = 0x38;
        o->pos_y = 0x20;
        o->field_09 = 4;
        o->field_01 = 1;
        func_80130768(o, 0, data_800502d4_slot28);
        o2 = data_80051ccc_slot28[1];
        o2->pos_x = 0x58;
        o2->pos_y = -0x60;
        o2->field_7a = 0x10;
        o2->field_7c = v;
        o2->field_0d = 0;
        o2->field_09 = 5;
        func_80130768(o2, 0, data_800502d4_slot28);
        func_8002725c_slot28(obj, 3);
        func_80128370();
    }
}

void func_80026f18_slot28(Object *obj) {
    Object *o = data_80051cdc_slot28.p;
    if ((s16)o->field_3a < 0) {
        data_8018f5a0->field_52++;
        func_8014f4d4(1, 0x601);
        func_80128370();
    }
}
