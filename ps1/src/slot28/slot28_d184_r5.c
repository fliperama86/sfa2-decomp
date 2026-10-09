/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8003c2f4_slot28[];
extern int data_8003c2f8_slot28[];

void func_8001da50_slot28(Object *obj);

void func_8001d9c8_slot28(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = obj->field_04 + 1;
    }
    func_80131094(obj);
}

void func_8001da08_slot28(Object *obj) {
    *(int *)&obj->field_10 = *(int *)&obj->field_10 + obj->field_4c;
    if (obj->pos_x >= 0x138) {
        func_8001da50_slot28(obj);
    }
}

void func_8001da50_slot28(Object *obj) {
    int v = func_80151184();
    int a = v & 0x7f;
    int k = data_8003c2f4_slot28[v & 3];
    int j = k - 3;
    obj->pos_x = v & 0x3f;
    obj->pos_y = 0x98 - a;
    obj->field_4c = data_8003c2f8_slot28[(j << 3) + (v & 7)];
    func_80130768(obj, k, (SequenceStep **)obj->box_tables);
}
