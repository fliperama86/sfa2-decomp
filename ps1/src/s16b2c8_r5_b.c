/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8016ced0(void (*cb)(void));
int func_8015783c(unsigned a, int b, int c, int d);
void func_8015f020(int a, void (*cb)(void));

int func_8016cf70(int a) {
    int v;

    if (data_80183194 == 1 || data_80183168 == 1) {
        return 1;
    }
    v = func_8015785c(data_801831ec);
    if (a == 1) {
        if (v == 0) {
            do {
                v = func_8015785c(data_801831ec);
            } while (v == 0);
        }
        v = 1;
        data_80183168 = v;
    } else if (v == 1) {
        data_80183168 = v;
    }
    return v;
}

int func_8016d018(int n, WordPair *p) {
    if (n > 0) {
        p->field_00 = 0x40001010;
        data_80183608 = p;
        data_80183604 = 0;
        data_80183600 = n;
        p->field_04 = (0x10000 << data_8018315c) - 0x1010;
        return n;
    }
    return 0;
}

void func_8016d06c(int a) {
    int v;

    switch (a) {
    case 0:
        v = 0;
        break;
    case 1:
        v = 1;
        break;
    default:
        v = 0;
        break;
    }
    data_80183194 = a;
    data_80183150 = v;
}

int func_8016d0a0(int a) {
    data_80183134 = func_8016c988(-1, a);
    return a;
}

int func_8016d0dc(int a, unsigned b) {
    if (b > 0x7f000) {
        b = 0x7f000;
    }
    func_8016c850(a, b);
    if (data_8018316c == 0) {
        data_80183168 = 0;
    }
    return b;
}

void func_8016d13c(int a) {
    if (a == 1) {
        data_80183168 = 0;
    } else {
        data_80183168 = 1;
    }
}

int func_8016d168(void) {
    return data_80183168 == 0;
}
