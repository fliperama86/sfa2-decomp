/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern s8 data_800f01fc_slot0f;
void func_800e0784_slot0f(int a);
void func_800e06b8_slot0f(GameState *g, int x);

void func_800e0560_slot0f(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_48++;
    h->field_4a = 0;
    h->field_4c = 0;
    func_80120408();
    func_8011eb14();
}

void func_800e05a0_slot0f(int a, int b) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    int hi;
    int lo;
    int v;
    if (b == 0) {
        data_800f01fc_slot0f = 0;
        return;
    }
    if (a == 0) {
        func_800e0784_slot0f(1);
    }
    if (data_800f01fc_slot0f & 0x80) {
        func_800e0784_slot0f(0);
    }
    v = data_800f01fc_slot0f;
    hi = v & 0x80;
    lo = v & 0x7f;
    lo++;
    if (lo == 0x20) {
        if (hi != 0) {
            lo = 0;
            hi = 0;
        } else {
            lo = 0x80;
        }
    }
    data_800f01fc_slot0f = hi | lo;
}

void func_800e0640_slot0f(GameState *g) {
    int x;
    if (g->field_4f != 0) {
        return;
    }
    x = 0;
    if (g->field_5c != 0) {
        g->field_5d--;
        x = 1;
        if (g->field_5d == 0) {
            x = 0;
            g->field_5c--;
            g->field_5d = g->field_5e;
        }
    }
    func_800e06b8_slot0f(g, x);
}
