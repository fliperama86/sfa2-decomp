/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801246cc(Object *a, Triple *b) {
    int i;
    TextBuf *t0 = &data_8016e8fc;
    TextBuf *t1 = &data_8016e92c;
    TextBuf *t2 = &data_8016e95c;
    TextBuf *t3 = &data_8016e98c;

    if (b->c == 0 && ((data_8019011f >> a->side) & 1)) {
        data_80185fb4 = data_8016e800;
        data_80185fb8 = b->c;
        func_80124bbc(a, b, 6);
        data_8016e800 = data_80185fb4;
        b->c = data_80185fb8;
        if (data_8016e800 == 4) {
            data_80185fb4 = data_8016e801;
            func_80124d70(a, b, 2);
            data_8016e801 = data_80185fb4;
            func_80124a7c(data_8016e801);
        }
        if (data_8016e800 == 5) {
            data_80185fb4 = data_8016e802;
            func_80124d70(a, b, 2);
            data_8016e802 = data_80185fb4;
            func_80124ae8(data_8016e802);
        }
        if (data_8016e800 == 6) {
            data_80185fb4 = data_8016e803;
            func_80124d70(a, b, 2);
            data_801a6989 = data_8016e803 = data_80185fb4;
        }
        if (b->c != 0 && data_8016e800 < 4) {
            func_80120554(0, 0, 0x34c);
        }
    }
    for (i = 0; i < 7; i++) {
        t0->buf[table_8016e810[i]] = 0x1a;
    }
    t0->buf[table_8016e810[data_8016e800]] = 0x14;
    if (data_801ae028 == 0) {
        t0->buf[table_8016e810[2]] = 0x1b;
    }
    for (i = 0; i < 3; i++) {
        t1->buf[table_8016e818[i]] = 0x1b;
    }
    t1->buf[table_8016e818[data_8016e801]] = 0x1a;
    for (i = 0; i < 3; i++) {
        t2->buf[table_8016e818[i]] = 0x1b;
    }
    t2->buf[table_8016e818[data_8016e802]] = 0x1a;
    for (i = 0; i < 3; i++) {
        t3->buf[table_8016e818[i]] = 0x1b;
    }
    t3->buf[table_8016e818[data_8016e803]] = 0x1a;
}
