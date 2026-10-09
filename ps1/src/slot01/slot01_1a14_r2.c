/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_800212ac_slot01[];
extern void (*data_80015320_slot01[])(Object *);
void func_80011f74_slot01(Object *obj);
void func_80012020_slot01(Object *obj);

void func_80011d00_slot01(Object *obj, u8 a) {
    func_80130768(obj, a, data_800212ac_slot01);
}

void func_80011d28_slot01(Object *obj) {
    if (!(obj->field_03 & 0x80)) {
        if (obj->field_03 == 0) {
            if (game_state.field_ab != 0) {
                obj->field_05 = 3;
            }
            data_80015320_slot01[obj->field_05](obj);
            func_80012020_slot01(obj);
        }
    }
    func_8011ffdc(obj);
}

void func_80011dc0_slot01(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t < 0) {
        obj->field_46 = 1;
        obj->field_05++;
        func_80011f74_slot01(obj);
    }
    obj->pos_x = obj->pos_x + obj->field_4c;
}
