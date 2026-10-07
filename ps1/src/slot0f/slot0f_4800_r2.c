/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Strip1c data_800f0264_slot0f[2][0x200];
extern int data_800f7268_slot0f;
void func_800e05a0_slot0f(int a, int b);

void func_800e483c_slot0f(void) {
    Rect rect;
    u8 *src;
    int i;
    int j;
    Chan *c = &data_801aa544[1];
    func_80136dc4();
    c->field_1e = 0x1800;
    c->field_58 = 0x400;
    c->field_5c = 0x100;
    c->field_8a = 0x1b;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x200; j++) {
            Strip1c *s = &data_800f0264_slot0f[i][j];
            func_80136d1c((Tx *)s);
            s->field_10 = 0x80;
            s->field_11 = 0x80;
            s->field_12 = 0x80;
        }
    }
    data_800f7268_slot0f = 1;
    func_800e05a0_slot0f(0, 0);
    src = (u8 *)0x800c7000;
    rect.x = 0x180;
    rect.y = 0;
    rect.w = 0x40;
    rect.h = 0x100;
    for (i = 0; i < 3; i++) {
        func_80157fc4(&rect, src);
        rect.x += 0x40;
        src += 0x8000;
    }
}
