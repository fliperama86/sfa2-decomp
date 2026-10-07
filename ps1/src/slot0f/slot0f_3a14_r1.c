/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u16 data_800f020c_slot0f[];
extern u16 data_800f022c_slot0f[];
extern void (*data_800e99a0_slot0f[])(void);

void func_800e3a14_slot0f(void) {
    Rect rect;
    int i;
    u16 *dst;
    u16 *src;
    int r;
    int g;
    int b;

    rect.x = 0x70;
    rect.y = 0x1fd;
    rect.w = 0x10;
    rect.h = 1;
    func_80158028(&rect, (u8 *)data_800f020c_slot0f);
    func_80157d9c(0);
    src = data_800f020c_slot0f;
    dst = data_800f022c_slot0f;
    for (i = 0; i < 0x10; src++, dst++, i++) {
        g = (*src >> 5) & 0x1f;
        b = *src & 0x1f;
        r = *src >> 10;
        r -= 7;
        if (r < 0) {
            r = 0;
        }
        g -= 7;
        if (g < 0) {
            g = 0;
        }
        b -= 7;
        if (b < 0) {
            b = 0;
        }
        *dst = b | ((r << 10) | (g << 5));
    }
    rect.x = 0x70;
    rect.y = 0x1ff;
    rect.w = 0x10;
    rect.h = 1;
    func_80157fc4(&rect, (u8 *)data_800f022c_slot0f);
    func_80157d9c(0);
}

void func_800e3b18_slot0f(void) {
    data_800e99a0_slot0f[((HudBig *)data_8018f5a0)->field_52]();
}
