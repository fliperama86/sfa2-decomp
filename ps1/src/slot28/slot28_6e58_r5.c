/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_80031c90_slot28[];
extern SequenceStep *data_80031c8c_slot28[];
extern SequenceStep *data_80031c9c_slot28[];
extern SequenceStep *data_80031ca0_slot28[];
extern SequenceStep *data_80031cc0_slot28[];
extern Object *data_80051910_slot28[];
extern int data_80051950_slot28;
Block172 *func_8011f1e0(void);
void func_801280f0(void);
void func_8012818c(void);
void func_80017974_slot28(Object *obj);
void func_800179b0_slot28(void);

void func_80017248_slot28(Object *obj) {
    Object *o;
    int one;
    HudState *h = data_8018f5a0;
    h->field_60 = 0x258;
    h->field_50 += 1;
    func_800179b0_slot28();
    func_801280f0();
    one = 1;
    data_80190568 = one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80017974_slot28(o);
        data_80051910_slot28[10] = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_80031c90_slot28);
        o->field_01 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80017974_slot28(o);
        data_80051910_slot28[15] = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_80031c8c_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80017974_slot28(o);
        data_80051910_slot28[12] = o;
        o->pos_x = 0x60;
        o->pos_y = -0x60;
        o->field_7a = 0x20;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 4;
        func_80130768(o, 0, data_80031c9c_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80017974_slot28(o);
        o->pos_x = 0xc0;
        o->pos_y = 0x90;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_80031ca0_slot28);
        data_80051910_slot28[0] = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80017974_slot28(o);
        data_80051910_slot28[14] = o;
        o->field_7c = 0x1e0;
        o->pos_x = 0x28;
        o->field_7a = 0;
        o->field_0d = 0;
        o->field_03 = one;
        o->field_09 = 0;
        o->field_50 = 0xb8;
        func_80130768(o, 0, data_80031cc0_slot28);
    }
    func_80151020(0x301);
    func_8012818c();
    data_80051950_slot28 = 0;
}
