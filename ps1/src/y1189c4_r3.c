/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


unsigned *func_80158374(unsigned *env);

void func_801194f4(void) {
    int idx;
    int i;
    char *old;
    char *buf;
    Pooled *p;
    void *s;
    idx = data_801a27d0 ^ 1;
    old = data_801987c8;
    buf = (char *)0x801fc000 + idx * 0xe8;
    data_801a27d0 = idx;
    data_801987c8 = buf;
    func_80158470(buf + 0x78);
    func_80158374((unsigned *)((char *)data_801987c8 + 0x8c));
    for (i = 0; i < 0x100; i++) {
        p = &data_8018e598[i];
        if (p->field_00 != 0) {
            func_80157fc4((char *)p + 8, p->field_04);
            func_8011f504(p);
        }
    }
    if (game_state.field_31 == 0) {
        func_80125b80(&game_state);
    }
    for (i = 0; i < 0x10; i++) {
        p = &data_8018f5e4[i];
        if (p->field_00 != 0) {
            func_80157fc4((char *)p + 8, p->field_04);
            func_8011f55c(p);
        }
    }
    if (data_8018daf4[0] != 0) {
        func_801378d8(&player_left);
    }
    if (data_8018daf4[1] != 0) {
        func_801378d8(&player_right);
    }
    s = old + 0x74;
    data_8018db14 = 0;
    data_801adfe4 = 0;
    func_80119694(s);
    func_80158300(s);
    func_80158208(data_801987c8, 0x1e);
    data_801901be = 0;
}
