/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

extern HudState *data_8018f5a0;
extern Object *data_80051998_slot28[];
extern ObjectRef data_800519c0_slot28;
extern Object *data_800519c4_slot28[];
extern SequenceStep *data_80036694_slot28[];
extern SequenceStep *data_800366a8_slot28[];
void func_801282d4(void);
void func_80128370(void);
void func_8001a958_slot28(Object *obj, int arg);
void func_8001a988_slot28(Object *obj);
void func_8001a9e8_slot28(void);

void func_8001a4e0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t == 0) {
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}

void func_8001a530_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        h->field_60 = 0x1e0;
        h->field_52 = h->field_52 + 1;
        func_8001a9e8_slot28();
        func_80130768(data_800519c0_slot28.p, 0, data_80036694_slot28);
        p = data_800519c4_slot28[0];
        p->pos_y = -0x50;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8001a988_slot28(p);
            p->pos_x = 0xd0;
            p->pos_y = 0x70;
            p->field_46 = 0x40;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 3;
            func_80130768(p, 4, data_800366a8_slot28);
            data_80051998_slot28[0] = p;
        }
        func_80120554(0, 0, 0x302);
        func_8001a958_slot28(obj, 3);
        func_80128370();
    }
}
