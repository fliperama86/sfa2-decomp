/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);
extern u16 data_801dd50c_slot05_06[];
extern u16 data_801dd51c_slot05_06[];
u8 func_80125734(Object *object, int a);
extern ObjectFn data_801dd528_slot05_06[];

void func_801cc8d8_slot05_06(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801cc90c_slot05_06(Object *obj) {
    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801cc95c_slot05_06(Object *obj) {
    int t;
    u16 v;
    u16 *p;

    obj->field_06 = obj->field_06 + 1;
    ((Slot04aObj *)obj)->field_47 = 0x3c;
    game_state.field_76 = 0x1e;
    obj->field_0b = obj->field_158;
    t = func_80151184() & 0x3f;
    p = &((Slot04aObj *)obj)->field_1c0;
    if (t == 0) {
        v = 4;
    } else {
        v = data_801dd50c_slot05_06[t & 7];
    }
    v = (s8)func_80125734(obj, (s8)v);
    *p = v + v;
    func_80130678(obj, data_801dd51c_slot05_06[v]);
}

void func_801cca20_slot05_06(Object *obj) {
    data_801dd528_slot05_06[((Slot04aObj *)obj)->field_1c0 >> 1](obj);
}

void func_801cca64_slot05_06(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
}
