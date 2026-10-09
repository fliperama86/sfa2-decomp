/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80120374(int a) {
    int x;
    if (a == 0) {
        func_8016582c();
        x = 0;
    } else {
        func_80165840();
        x = 1;
    }
    func_8014f5d4(x);
}

void func_801203b4(int a) {
    func_8016a3b0(data_80190a44[a]);
    func_8016a70c(data_80190a44[a + 8]);
}

void func_80120408(void) {
    s8 *p;
    int i;
    func_80164bbc(0);
    for (i = 23, p = data_80190a44 + 23; i >= 0; i--, p--) {
        p[0x748c] = 0;
    }
}

void func_80120444(int a, unsigned char b) {
    func_8016936c(data_80190a44[a + 8], table_8016e6e0[a][b * 2] & 0xfff);
}

void func_80120498(int a) {
    int hi = (a & 0xff00) >> 8;
    func_8016936c(data_80190a44[hi + 8], table_8016e6e0[hi][(a & 0xff) * 2] & 0xfff);
}

void func_801204f4(Object *o, int b, int c) {
    int d = o->pos_x - (s16)box_margin[0];
    if (d < -192) {
        d = -192;
    } else if (d >= 576) {
        d = 575;
    }
    func_80120604(b, b, c & 0xff, d);
}

void func_80120554(Object *o, int b, unsigned c) {
    int d;
    if (o != 0) {
        d = o->pos_x - (s16)box_margin[0];
        if (d < -192) {
            d = -192;
        } else if (d >= 576) {
            d = 575;
        }
    } else {
        d = 192;
    }
    func_80120604((c & 0xff00) >> 8, b, c & 0xff, d);
}

void func_801205c4(int a, unsigned c) {
    int d;
    if (a == 0) {
        d = 64;
    } else {
        d = 320;
    }
    func_80120604((c & 0xff00) >> 8, a, c & 0xff, d);
}
