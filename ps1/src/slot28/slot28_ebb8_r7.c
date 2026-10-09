/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051b00_slot28[];
extern Object *data_80051b04_slot28[];
extern ObjectRef data_80051b08_slot28;
extern Object *data_80051ad8_slot28[];
extern SequenceStep *data_80041498_slot28[];
extern SequenceStep *data_800414a4_slot28[];
extern SequenceStep *data_800414d4_slot28[];
extern u8 data_8003f9fc_slot28[];
extern u8 data_8003feb8_slot28[];
void func_800200ec_slot28(void);
void func_8002005c_slot28(Object *obj, int arg);

void func_8001f698_slot28(Object *obj) {
    HudState *h;
    Object *p;
    int x;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52++;
        func_800200ec_slot28();
        func_80130768(data_80051b00_slot28[0], 2, data_80041498_slot28);
        x = 0x38;
        p = data_80051b04_slot28[0];
        p->pos_x = x;
        p->pos_y = 0x20;
        func_80130768(p, 2, data_800414a4_slot28);
        p = data_80051b08_slot28.p;
        p->pos_x = x;
        p->pos_y = -0x60;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x71;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8003f9fc_slot28;
            p->field_9c = data_8003feb8_slot28;
            p->pos_x = 0x40;
            p->pos_y = 0xcc;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 2;
            p->field_03 = 0;
            p->field_01 = 0;
            p->field_0d = 0;
            data_80051ad8_slot28[0] = p;
            ((Slot28Obj *)p)->field_6c = data_800414d4_slot28;
        }
        func_80120554(0, 0, 0x304);
        func_80128370();
        func_8002005c_slot28(obj, 1);
    }
}
