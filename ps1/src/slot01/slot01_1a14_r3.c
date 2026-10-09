/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern Slot01State55e94 data_80055e94_slot01;
extern Slot01Rec152ec data_800152e4_slot01;
void func_80011fe8_slot01(Object *obj, Slot01Rec152ec *a1);
void func_80120028(Object *obj);

void func_80011e28_slot01(Object *obj) {
    Object *o = obj;
    int t = o->field_46 - 1;
    o->field_46 = t;
    if ((s16)t < 0) {
        o->field_46 = 1;
        o->field_4c = o->field_4c + o->field_54;
        if (o->field_4c == 0) {
            o->field_05++;
            ((Object *)o->field_28)->field_04 = 2;
        }
        o->pos_x = (o->field_4c & 2) ? o->pos_x - o->field_4c : o->pos_x + o->field_4c;
    }
}

void func_80011eb8_slot01(Object *obj) {
    if (data_80055e94_slot01.value & 0x8000) {
        obj->field_04++;
    }
}

void func_80011ee8_slot01(Object *obj) {
    obj->field_04++;
    obj->pos_x = box_margin[0] + 5;
    ((Object *)obj->field_28)->field_04 = 2;
    func_80011fe8_slot01(obj, &data_800152e4_slot01);
}

void func_80011f34_slot01(Object *obj) {
    func_80120028(obj);
}
