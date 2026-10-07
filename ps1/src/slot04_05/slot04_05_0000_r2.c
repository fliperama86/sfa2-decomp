/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c1340_slot04_05[];
extern ObjectFn data_801c1350_slot04_05[];

u8 func_80125734(Object *object, int a);
void func_80130678(Object *object, int index);

void func_801b0224_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u8 r;

    o->field_07 = 0;
    o->field_46 = 0x3c;
    o->field_06 = o->field_06 + 1;
    game_state.field_76 = 0x1e;
    o->field_0b = o->field_158;
    r = func_80125734(o, data_801c1340_slot04_05[func_80151184() & 0xf]);
    obj->field_1a7 = r;
    func_80130678(o, r + 0x23);
}

void func_801b02a8_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    data_801c1350_slot04_05[obj->field_1a7](o);
}

void func_801b02e8_slot04_05(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
}
