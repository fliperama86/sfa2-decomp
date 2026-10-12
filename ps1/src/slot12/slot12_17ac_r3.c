/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn table_80022dd8_slot12[];
extern Slot12List data_80022dc4_slot12;
void func_800119cc_slot12(Object *obj);
void func_80011bd8_slot12(Object *obj, Slot12List *list);

void func_800118f0_slot12(Object *obj) {
    table_80022dd8_slot12[obj->state_1](obj);
    if (obj->table_index != 0) {
        func_8011ffdc(obj);
    } else {
        func_80011bd8_slot12(obj, &data_80022dc4_slot12);
    }
}

void func_8001196c_slot12(Object *obj) {
    int d = 0xfffc;
    int x = obj->field_20;
    int y = obj->y_scale;
    y += d;
    x += d;
    obj->y_scale = y;
    obj->field_20 = x;
    if ((u16)y != 0) {
        func_800119cc_slot12(obj);
    } else {
        obj->state_1++;
    }
}

void func_800119cc_slot12(Object *obj) {
    if (game_state.status_flags != 0) {
        obj->countdown = 0x1f;
        obj->state_1++;
    }
}
