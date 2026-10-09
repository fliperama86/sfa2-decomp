/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015870_slot01[];
extern ObjectFn data_8001587c_slot01[];
void func_80014c3c_slot01(void);

Poly28 *func_80012f60_slot01(Object *obj, Poly28 *p, int a2, int a3) {
    int x = a2;
    int y = a3;
    u32 *d = (u32 *)obj->sequence->field_04;
    int i;
    int n;

    for (i = 0; i < 2; i++) {
        n = *(s16 *)d;
        do {
            func_8015c09c(p);
            p->field_04 = 0x80;
            p->field_05 = 0x80;
            p->field_06 = 0x80;
            p->field_16 = x;
            p->field_0e = y;
            p++;
        } while (--n > 0);
    }
    return p;
}

void func_8001301c_slot01(Object *obj) {
    if (obj->field_03 & 0x80) {
        data_8001587c_slot01[obj->field_04](obj);
    } else {
        data_80015870_slot01[obj->field_04](obj);
        func_80014c3c_slot01();
    }
}
