/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051df0_slot28;
extern u16 data_801a6966;
void func_800279d4_slot28(Object *o);

void func_80027748_slot28(int *p) {
    u16 *a = &data_801a6966;
    u16 *b = &data_801a6972;
    Object *o = data_80051df0_slot28.p;
    p[1] += p[0];
    *(s32 *)&o->field_14 -= p[0];
    if (((*a | *b) & 0x40) != 0) {
        p[1] += p[0];
        p[1] += p[0];
        p[1] += p[0];
        *(s32 *)&o->field_14 -= p[0];
        *(s32 *)&o->field_14 -= p[0];
        *(s32 *)&o->field_14 -= p[0];
    }
    if (o->field_48 == 0xff) {
        if (((*a | *b) & 0x40) != 0) {
            p[1] -= p[0];
            p[1] -= p[0];
            p[1] -= p[0];
        } else {
            p[1] -= p[0];
        }
        data_8018f5a0->field_60 = 0xb4;
        data_8018f5a0->field_50++;
    }
    func_800279d4_slot28((Object *)p);
    func_80138164();
    func_8011abe4();
}
