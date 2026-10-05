/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801541e0(Object *o) {
    u8 d[2];
    int i;
    unsigned n;
    TextObj *t;
    n = o->field_d7;
    if (n != 0) {
        t = table_80180f5c[o->side];
        if (n / 10 * 10 == n - 1) {
            t->buf[5] = 0x20;
        } else {
            t->buf[5] = 0x53;
        }
        i = 0;
        do {
            d[i] = n % 10;
            n /= 10;
            i++;
        } while (i < 2);
        n = d[1];
        if (n != 0) {
            t->buf[0] = n + 0x30;
        } else {
            t->buf[0] = 0x20;
        }
        t->buf[1] = d[0] + 0x30;
        func_801519b4(t);
    }
}
