/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Poly28 data_80051df4_slot28[];
int func_8015bdd4(int a, int b);
void func_8015c09c(void *prim);

void func_80028038_slot28(Object *obj) {
    int i;

    Poly28 *p = data_80051df4_slot28;

    for (i = 0; i < 2; p++, i++) {
        func_8015c09c(p);
        p->field_04 = 0x80;
        p->field_05 = 0x80;
        p->field_06 = 0x80;
        p->field_16 = func_8015bd0c(1, 0, 0x180, 0);
        p->field_0e = func_8015bdd4(0x180, 0xff);
        p->field_08 = 0;
        p->field_0a = 0;
        p->field_10 = 0x100;
        p->field_12 = 0;
        p->field_18 = 0;
        p->field_1a = 0xf0;
        p->field_20 = 0x100;
        p->field_22 = 0xf0;
        p->field_0c = 0;
        p->field_0d = 0;
        p->field_14 = 0xff;
        p->field_15 = 0;
        p->field_1c = 0;
        p->field_1d = 0xf0;
        p->field_24 = 0xff;
        p->field_25 = 0xf0;
    }
    p = &data_80051df4_slot28[2];
    for (i = 0; i < 2; p++, i++) {
        func_8015c09c(p);
        p->field_04 = 0x80;
        p->field_05 = 0x80;
        p->field_06 = 0x80;
        p->field_16 = func_8015bd0c(1, 0, 0x200, 0);
        p->field_0e = func_8015bdd4(0x180, 0xff);
        p->field_08 = 0x100;
        p->field_0a = 0;
        p->field_10 = 0x180;
        p->field_12 = 0;
        p->field_18 = 0x100;
        p->field_1a = 0xf0;
        p->field_20 = 0x180;
        p->field_22 = 0xf0;
        p->field_0c = 0;
        p->field_0d = 0;
        p->field_14 = 0x80;
        p->field_15 = 0;
        p->field_1c = 0;
        p->field_1d = 0xf0;
        p->field_24 = 0x80;
        p->field_25 = 0xf0;
    }
}
