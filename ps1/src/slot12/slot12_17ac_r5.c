/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Bank data_80028b48_slot12[];

void func_80011a78_slot12(Object *obj, Slot12List *list) {
    Cell20 *a = data_80028b48_slot12[0].cells;
    Cell20 *b = data_80028b48_slot12[1].cells;
    int i;
    int t;
    u16 n = list->count;

    for (i = 0; i < n; i++) {
        func_8015c100(a);
        func_8015c100(b);
        a->field_04 = 0x80;
        b->field_04 = 0x80;
        a->field_05 = 0x80;
        b->field_05 = 0x80;
        a->field_06 = 0x80;
        b->field_06 = 0x80;
        t = i / 16 * 16;
        a->field_0c = (i - t) << 4;
        b->field_0c = (i - t) << 4;
        a->field_0d = t;
        b->field_0d = t;
        a->field_0e = func_8015bdd4((s16)obj->field_7a, (s16)obj->field_7c);
        b->field_0e = func_8015bdd4((s16)obj->field_7a, (s16)obj->field_7c);
        a->field_10 = 0x10;
        b->field_10 = 0x10;
        a->field_12 = 0x10;
        b->field_12 = 0x10;
        a++;
        b++;
    }
    func_80158a2c((Prim *)a, 0, 0, 0x16, 0);
    func_80158a2c((Prim *)b, 0, 0, 0x16, 0);
}
