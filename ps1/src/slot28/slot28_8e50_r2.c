/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051980_slot28[];
extern ObjectRef data_80051994_slot28;

void func_80019090_slot28(Object *obj) {
    Object *p = data_80051980_slot28[0];
    int t = *(u16 *)&p->pos_x + 1;
    p->pos_x = t;
    if ((s16)t >= 0x160) {
        p->pos_x = -0xa0;
    }
    p = data_80051994_slot28.p;
    t = *(u16 *)&p->pos_x + 1;
    p->pos_x = t;
    if ((s16)t >= 0x160) {
        p->pos_x = -0xa0;
    }
}
