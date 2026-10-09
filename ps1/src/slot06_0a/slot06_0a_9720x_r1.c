/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed400_slot06_0a[];
extern u8 data_801ed490_slot06_0a[];

void func_801e9720_slot06_0a(Object *obj) {
    Object *t = (Object *)obj->field_4c;

    if (t != 0) {
        u16 a;
        u16 d;
        int off = 0x18;

        /* One local `a` holds the target's x and later the absolute value: with the x read at its use eight instruction slots differ. */
        a = t->pos_x;
        if (obj->field_03 != 0) {
            off = -0x18;
        }
        d = a + off - (u16)obj->pos_x;
        a = d;
        if (d & 0x8000) {
            a = -d;
        }
        if (a < 0x81) {
            obj->field_05 = 1;
            func_80130700(obj, data_801ed400_slot06_0a[data_801ed490_slot06_0a[obj->field_03]]);
        }
    }
}
