/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c56a4_slot04_06[];

void func_801b6334_slot04_06(Object *obj) {
    if (game_state.field_6a != 0) {
        obj->field_04 = 2;
    } else {
        data_801c56a4_slot04_06[obj->field_03](obj);
    }
}

void func_801b638c_slot04_06(Object *obj) {
    int t;
    int a;

    t = obj->field_48 + 1;
    obj->field_48 = t & 0x1f;
    a = 1;
    if (t & 0x10) {
        a = -1;
    }
    obj->pos_y -= a;
    func_80131094(obj);
    func_8011ffdc(obj);
}
