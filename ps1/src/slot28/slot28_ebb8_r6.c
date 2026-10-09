/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051b00_slot28[];
extern Object *data_80051b04_slot28[];
extern ObjectRef data_80051b08_slot28;
extern ObjectRef data_80051b10_slot28;
extern ObjectRef data_80051b14_slot28;
extern SequenceStep *data_80041498_slot28[];
extern SequenceStep *data_80041494_slot28[];
extern SequenceStep *data_800414a4_slot28[];
extern SequenceStep *data_800414b0_slot28[];
extern SequenceStep *data_800414c0_slot28[];
void func_800200c8_slot28(void);
void func_8002008c_slot28(Object *obj);

void func_8001f378_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *p;
    int one;
    h->field_60 = 0x258;
    h->field_50++;
    func_80151020(0x500);
    one = 1;
    func_800200c8_slot28();
    func_801280f0();
    data_80190568 = one;
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002008c_slot28(p);
        data_80051b00_slot28[0] = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 1, data_80041498_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002008c_slot28(p);
        data_80051b14_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 0, data_80041494_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002008c_slot28(p);
        data_80051b04_slot28[0] = p;
        p->pos_x = 0x60;
        p->pos_y = 0x30;
        p->field_7a = 0x10;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 3;
        func_80130768(p, 0, data_800414a4_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002008c_slot28(p);
        data_80051b08_slot28.p = p;
        p->pos_x = 0x58;
        p->pos_y = 0x20;
        p->field_7a = 0x20;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 4;
        func_80130768(p, 0, data_800414b0_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002008c_slot28(p);
        data_80051b10_slot28.p = p;
        p->field_7c = 0x1e0;
        p->pos_x = 0x28;
        p->field_7a = 0;
        p->field_0d = 0;
        p->field_03 = one;
        p->field_09 = 0;
        p->field_50 = 0xb8;
        func_80130768(p, 0, data_800414c0_slot28);
    }
    func_8012818c();
}
