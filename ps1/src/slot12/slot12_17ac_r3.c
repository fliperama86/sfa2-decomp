/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn table_80022dd8_slot12[];
extern Slot12List data_80022dc4_slot12;
void func_800119cc_slot12(Object *obj);
void func_80011bd8_slot12(Object *obj, Slot12List *list);

void func_800118f0_slot12(Object *obj) {
    table_80022dd8_slot12[obj->field_05](obj);
    if (obj->field_03 != 0) {
        func_8011ffdc(obj);
    } else {
        func_80011bd8_slot12(obj, &data_80022dc4_slot12);
    }
}

void func_8001196c_slot12(Object *obj) {
    int d = 0xfffc;
    int x = obj->field_20;
    int y = obj->field_22;
    y += d;
    x += d;
    obj->field_22 = y;
    obj->field_20 = x;
    if ((u16)y != 0) {
        func_800119cc_slot12(obj);
    } else {
        obj->field_05++;
    }
}

void func_800119cc_slot12(Object *obj) {
    if (game_state.field_ab != 0) {
        obj->field_46 = 0x1f;
        obj->field_05++;
    }
}
