/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80153cac(Actor *a) {
    u8 d[2];
    int i;
    u8 c;
    unsigned n;
    TextObj *t;
    if (a->field_09 >= 100) {
        a->field_09 = 99;
    }
    t = table_801803c4[a->field_01];
    n = a->field_09;
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
