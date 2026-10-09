/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c5510_slot04_06[];
extern u16 data_801c5520_slot04_06[];
extern ObjectFn data_801c552c_slot04_06[];

u8 func_80125734(Object *object, int a);

void func_801b4960_slot04_06(Object *obj) {
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
        v = data_801c5510_slot04_06[t & 7];
    }
    v = (s8)func_80125734(obj, (s8)v);
    *p = v + v;
    func_80130678(obj, data_801c5520_slot04_06[v]);
}

void func_801b4a24_slot04_06(Object *obj) {
    data_801c552c_slot04_06[((Slot04aObj *)obj)->field_1c0 >> 1](obj);
}
