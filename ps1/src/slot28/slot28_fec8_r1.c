/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051b04_slot28[];
extern ObjectRef data_80051b08_slot28;
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_8002005c_slot28(Object *obj, int arg);

void func_8001fec8_slot28(Object *obj) {
    HudState *h;
    Object *b;

    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52++;
        b = data_80051b04_slot28[0];
        b->field_01 = 0;
        b = data_80051b08_slot28.p;
        b->pos_x = 0x58;
        b->pos_y = -0x60;
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0xa6;
            b->field_03 = 9;
            b->field_01 = 0;
        }
        func_8002005c_slot28(obj, 4);
        func_80128370();
    }
}
