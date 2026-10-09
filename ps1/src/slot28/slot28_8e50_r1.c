/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051980_slot28[];
extern ObjectRef data_80051984_slot28;
extern ObjectRef data_80051994_slot28;
extern Object *data_80051954_slot28[];
extern SequenceStep *data_80034520_slot28[];
extern SequenceStep *data_80034528_slot28[];
extern SequenceStep *data_80034530_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_80019190_slot28(void);
void func_80019130_slot28(Object *obj);
void func_80019100_slot28(Object *obj, int arg);

void func_80018e50_slot28(Object *obj) {
    Object *o;
    Object *n;
    int one;
    int x60;

    if (data_80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52++;
        func_80019190_slot28();
        x60 = 0x60;
        o = data_80051980_slot28[0];
        o->pos_x = x60;
        o->pos_y = -0x60;
        func_80130768(o, 1, data_80034520_slot28);
        o->pos_x -= 1;
        o = data_80051994_slot28.p;
        o->field_01 = 0;
        o = data_80051984_slot28.p;
        o->pos_x = 0x80;
        o->pos_y = 0x20;
        func_80130768(o, 1, data_80034528_slot28);
        one = 1;
        o->field_01 = one;
        n = (Object *)func_8011f1e0();
        if (n != 0) {
            func_80019130_slot28(n);
            n->pos_x = 0xc8;
            n->pos_y = 0xa0;
            n->field_7a = x60;
            n->field_7c = 0x1e0;
            n->field_0d = 0;
            n->field_09 = one;
            n->field_48 = 0;
            func_80130768(n, 9, data_80034530_slot28);
            data_80051954_slot28[0] = n;
        }
        func_80128370();
        func_80019100_slot28(obj, 7);
    }
}
