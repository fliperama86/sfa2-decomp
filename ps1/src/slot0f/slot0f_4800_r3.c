/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Strip1c data_800f0264_slot0f[2][0x200];
extern s16 data_800f7264_slot0f;
extern int data_800f7268_slot0f;
extern u16 data_800e99b0_slot0f[];
void func_800e05a0_slot0f(int a, int b);
void func_800e4ab0_slot0f(Chan *c, Strip1c *buf, s16 count);

void func_800e4988_slot0f(void) {
    Chan *c;
    data_800f7268_slot0f--;
    if (data_800f7268_slot0f == 0) {
        func_80120554(0, 0, 0x200);
        data_800f7268_slot0f = 0xe10;
    }
    func_800e05a0_slot0f(0, 1);
    c = &data_801aa544[1];
    *(u32 *)&c->field_10 = 0x2500000;
    c->field_14 = 0x80000;
    data_800f7264_slot0f = 0;
    c->field_50 = data_800e99b0_slot0f;
    func_800e4ab0_slot0f(c, data_800f0264_slot0f[data_801a27d0], 0x18);
    *(u32 *)&c->field_10 = 0;
    c->field_14 = 0;
    c->field_50 = data_800e99b0_slot0f;
    func_800e4ab0_slot0f(c, &data_800f0264_slot0f[data_801a27d0][data_800f7264_slot0f], 0x18);
}
