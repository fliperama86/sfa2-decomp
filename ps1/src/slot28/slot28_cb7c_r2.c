/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8003b504_slot28[];
extern SequenceStep *data_8003b50c_slot28[];
extern ObjectRef data_80051a40_slot28;
extern Object *data_80051a44_slot28[];
void func_8001d548_slot28(Object *obj, int arg);
void func_8001d578_slot28(Object *obj);

void func_8001cde0_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_52 += 1;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8001d578_slot28(p);
            data_80051a44_slot28[0] = p;
            p->pos_x = 0x48;
            p->pos_y = -0x60;
            p->field_7a = 0x10;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 5;
            func_80130768(p, 0, data_8003b50c_slot28);
        }
        p = data_80051a44_slot28[0];
        p->pos_y = 0x20;
        p->field_09 = 5;
        func_80130768(p, 0, data_8003b50c_slot28);
        func_80130768(data_80051a40_slot28.p, 1, data_8003b504_slot28);
        func_8001d548_slot28(obj, 1);
        func_8012818c();
    }
}
