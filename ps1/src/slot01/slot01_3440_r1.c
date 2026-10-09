/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01Rec31b98 data_80031b98_slot01[];

void func_80013440_slot01(Object *obj) {
    Cell20 *a = data_80031b98_slot01[0].cells;
    Cell20 *b = data_80031b98_slot01[1].cells;
    int i;
    int j;

    obj->field_01 = 0;
    obj->field_5c = 0;
    ((Slot01Obj *)obj)->field_5e = -0x1f;
    obj->field_04 = obj->field_04 + 1;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 3; j++) {
            func_8015c100(a);
            a->field_08 = j << 7;
            a->field_0a = i << 5;
            a->field_0c = 0;
            a->field_0d = 0;
            a->field_10 = 0x80;
            a->field_04 = 0x80;
            a->field_05 = 0x80;
            a->field_06 = 0x80;
            a->field_12 = 0x20;
            a->field_0e = func_8015bdd4(0x10, i + 0x1e0);
            a++;
            func_8015c100(b);
            b->field_08 = j << 7;
            b->field_0a = i << 5;
            b->field_0c = 0;
            b->field_0d = 0;
            b->field_10 = 0x80;
            b->field_04 = 0x80;
            b->field_05 = 0x80;
            b->field_06 = 0x80;
            b->field_12 = 0x20;
            b->field_0e = func_8015bdd4(0x10, i + 0x1e0);
            b++;
        }
    }
    func_80158a2c((Prim *)a, 0, 0, 0x1a, 0);
    func_80158a2c((Prim *)b, 0, 0, 0x1a, 0);
}
