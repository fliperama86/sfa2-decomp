/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec50ce4 data_80050ce4_slot28[];
extern s16 data_8005175c_slot28[];

void func_800279d4_slot28(Object *o) {
    Slot28Rec50ce4Obj *obj = (Slot28Rec50ce4Obj *)o;
    int i;
    int x;
    s16 h;
    Slot28Rec50ce4 *e;
    Slot28Rec51e94 *c;
    i = 0;
    if (obj->field_08->field_00 != 0xff) {
        h = obj->field_06;
        obj->field_08 = &data_80050ce4_slot28[((h & 0xff00) >> 4) + ((u32)(h & 0xff) >> 4)];
    }
    x = 8 - ((u16)obj->field_06 & 0xf);
    e = obj->field_08;
    do {
        c = &obj->cells[i];
        if (e->field_00 != 0xff) {
            c->field_04 = e->field_00;
            c->field_06 = x;
            c->field_0b = e->field_02;
            c->field_0c = e->field_04;
            e++;
            func_801519b4((Object *)c);
        }
        x += 0x10;
        i++;
    } while (i < 0x10);
}

void func_80027adc_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    func_80151020(data_8005175c_slot28[obj->field_70]);
}
