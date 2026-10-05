/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void *func_8015783c(int a, int b, int c, void (*d)(void));

void func_80118d10(short a, short b) {
    int v;
    int *p;
    func_8015efc0();
    func_8015f0b4();
    while (func_8014f0bc() == 0) {
    }
    func_80157d00(0);
    func_801578fc(0);
    func_8015f734(0);
    p = (int *)0x1f800000;
    v = *p;
    data_801abef8 = a;
    data_801ac314 = b;
    if (v < 0) {
        func_8012028c();
    }
    func_80120168();
    func_8015762c(&data_801a89a8, 4, &data_801a89ac, 4);
    func_801577dc();
    func_801542ec();
    func_801577fc(0);
    *p = 0;
    scratch_word_04 = 0;
    scratch_word_10 = 0;
    func_801575dc();
    data_801900fc = func_8015783c(0xf2000003, 2, 0x1000, func_80119444);
    func_8015786c();
    func_8015761c(data_801900fc);
    func_8015764c(0xf2000003, 1, 0x1000);
    func_80157720(0xf2000003);
}



void func_80118e58(int a, int b, int c, int d) {
    Rect rect;
    void *buf;
    func_8015c5f0((void *)0x801fc08c, 0, 8, c, d);
    func_8015c6b0((void *)0x801fc078, 0, 0xf0, a, b);
    func_8015c5f0((void *)0x801fc174, 0, 0xf8, c, d);
    func_8015c6b0((void *)0x801fc160, 0, 0, a, b);
    buf = (void *)0x801fc000;
    data_801fc18c = 1;
    data_801fc0a4 = 1;
    data_801fc18a = 1;
    data_801fc0a2 = 1;
    rect.w = 0x3ff;
    data_801fc0a5 = 0;
    data_801fc0a6 = 0;
    data_801fc0a7 = 0;
    data_801fc18d = 0;
    data_801fc18e = 0;
    data_801fc18f = 0;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1ff;
    func_80157f30(&rect, 0, 0, 0);
    func_80158208((void *)0x801fc000, 0x1e);
    func_80158208((void *)0x801fc0e8, 0x1e);
    data_801a27d0 = 0;
    data_801987c8 = buf;
}
