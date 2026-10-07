/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Count data_800f7b74_slot0f[4];
extern ObjectRef data_80190468;
extern void (*data_800eff78_slot0f[])(Object *);

void func_800e6ce4_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u8 *p;
    int n;
    int i;
    int v;

    data_800f7b74_slot0f[2].value = 0;
    p = &data_80190468.p->field_15e;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            v = *p++ & 0x7f;
            if (v >= 0x10) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_800f7b74_slot0f[2].value = n;
}

void func_800e6d44_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u8 *p;
    int n;
    int i;
    int v;

    data_800f7b74_slot0f[3].value = 0;
    p = &data_80190468.p->field_15e;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            v = *p++ & 0x7f;
            if ((v == 7) | (v == 0xb)) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_800f7b74_slot0f[3].value = n;
}

void func_800e6db4_slot0f(Object *obj) {
}

void func_800e6dbc_slot0f(Object *object) {
    data_800eff78_slot0f[object->field_05](object);
}
