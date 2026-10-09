/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_800519d8_slot28[];
extern ObjectRef data_80051a00_slot28;
extern Object *data_80051a04_slot28[];
extern SequenceStep *data_80039280_slot28[];
extern SequenceStep *data_80039290_slot28[];
extern SequenceStep *data_8003929c_slot28[];
extern u16 data_8003a04c_slot28[];
void func_8001c430_slot28(Object *obj, int arg);
void func_8001c460_slot28(Object *obj);
void func_8001c4c0_slot28(void);

void func_8001bb30_slot28(Object *obj) {
    int i;
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_8001c4c0_slot28();
        func_80130768(data_80051a00_slot28.p, 1, data_80039280_slot28);
        p = data_80051a04_slot28[0];
        p->pos_x = 0x60;
        p->pos_y = -0x50;
        func_80130768(p, 0, data_80039290_slot28);
        for (i = 0; i < 7; i++) {
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                func_8001c460_slot28(p);
                p->pos_x = data_8003a04c_slot28[i * 2];
                p->pos_y = data_8003a04c_slot28[i * 2 + 1];
                p->field_7a = 0x60;
                p->field_7c = 0x1e0;
                p->field_0d = 0;
                p->field_09 = 2;
                func_80130768(p, 3, data_8003929c_slot28);
                data_800519d8_slot28[i] = p;
            }
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8001c460_slot28(p);
            p->pos_x = 0xc0;
            p->pos_y = 0x81;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 4, data_8003929c_slot28);
            data_800519d8_slot28[7] = p;
        }
        func_8001c430_slot28(obj, 3);
        func_80128370();
    }
}
