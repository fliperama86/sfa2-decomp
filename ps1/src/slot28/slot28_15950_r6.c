/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8004ea70_slot28[];
extern int data_8004ea74_slot28[];
void func_80026368_slot28(Object *obj);

void func_80026320_slot28(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    if (obj->pos_x >= 0x140) {
        func_80026368_slot28(obj);
    }
}

void func_80026368_slot28(Object *obj) {
    int r = func_80151184() & 0xff;
    int k = data_8004ea70_slot28[r & 3];
    int row = k - 6;
    obj->pos_x = r & 0x3f;
    obj->pos_y = 0x98 - (r & 0x7f);
    obj->field_4c = data_8004ea74_slot28[row * 8 + (r & 7)];
    func_80130768(obj, k, (SequenceStep **)obj->box_tables);
}
