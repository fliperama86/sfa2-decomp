/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01Rec33bb4 data_80033bb4_slot01[2][2500];
void func_80014dd0_slot01(Tx *tx);

void func_80014c8c_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    int i;
    int j;
    Slot01Rec33bb4 *r;
    Rect rect;

    o->field_50 = 0x80093000;
    o->field_1e = 0x5800;
    o->field_58 = 0x400;
    *(u32 *)&o->field_5c = 0x100;
    o->field_04++;
    obj->field_14 = 0;
    obj->field_8a = 0x10;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x118; j++) {
            r = &data_80033bb4_slot01[i][j];
            func_80014dd0_slot01((Tx *)r);
            r->field_10 = 0x80;
            r->field_11 = 0x80;
            r->field_12 = 0x80;
        }
    }
    rect.x = 0;
    rect.y = 0x1e0;
    rect.w = 0x10;
    rect.h = 0x20;
    func_80157fc4(&rect, (u8 *)0x80095000);
    rect.x = 0x2c0;
    rect.y = 0x100;
    rect.w = 0x40;
    rect.h = 0x100;
    func_80157fc4(&rect, (u8 *)0x80095400);
    func_80157d9c(0);
}

void func_80014dd0_slot01(Tx *tx) {
    func_80158a2c((Prim *)tx, 0, 0, 0, 0);
    tx->field_0f = 3;
    tx->field_13 = 0x74;
    func_8015c23c(tx, &tx->field_0c);
}
