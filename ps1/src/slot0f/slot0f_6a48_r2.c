/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Count data_800f7b74_slot0f[4];
void func_800e6c18_slot0f(Object *obj);
void func_800e6c74_slot0f(Object *obj);
void func_800e6ce4_slot0f(Object *obj);
void func_800e6d44_slot0f(Object *obj);

void func_800e6b8c_slot0f(char *src, char *dst) {
    if (*src != 0) {
        *dst++ = 0x78;
        do {
            *dst++ = *src++;
        } while (*src != 0);
    }
    *dst++ = 0x40;
    *dst = 0x40;
    dst[1] = 0;
}

void func_800e6bd8_slot0f(Object *obj) {
    func_800e6c18_slot0f(obj);
    func_800e6c74_slot0f(obj);
    func_800e6ce4_slot0f(obj);
    func_800e6d44_slot0f(obj);
}

void func_800e6c18_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u8 *p;
    int n;
    int i;

    data_800f7b74_slot0f[0].value = 0;
    p = &data_80190468.p->field_15e;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            if (*p++ & 0x80) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_800f7b74_slot0f[0].value = n;
}

void func_800e6c74_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u8 *p;
    int n;
    int i;
    int v;

    data_800f7b74_slot0f[1].value = 0;
    p = &data_80190468.p->field_15e;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            v = *p++ & 0x7f;
            if ((unsigned)(v - 0xc) < 2 | (unsigned)(v - 0xe) < 2) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_800f7b74_slot0f[1].value = n;
}
