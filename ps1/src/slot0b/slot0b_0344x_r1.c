/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e4704_slot0b[])(Object *);
extern Slot0bPair data_801e46cc_slot0b[];
extern Slot0bPair data_801e46d4_slot0b[];
extern u8 *data_801e46b4_slot0b[];
int func_8015bdd4(int a, int b);
void func_801e0ce8_slot0b(Object *obj, u8 *a, int b, int c, int d);

void func_801e0344_slot0b(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int s;
    int s2;
    int r;
    int side;

    obj->field_20 = 0xfff0;
    obj->field_22 = 0xfff0;
    obj->field_0e = 0;
    obj->field_09 = 0;
    obj->field_24 = 0;
    obj->field_04 = obj->field_04 + 1;
    data_801e4704_slot0b[obj->field_03](obj);
    ref_other.p = obj->field_3c;
    s = ref_other.p->field_d4;
    if (ref_other.p->side == 1) {
        s += 6;
    }
    side = ref_other.p->side;
    s2 = func_8015bd0c(0, 0, data_801e46cc_slot0b[side].a, data_801e46cc_slot0b[side].b) & 0xffff;
    r = func_8015bdd4(data_801e46d4_slot0b[s].a, data_801e46d4_slot0b[s].b);
    func_801e0ce8_slot0b(obj, data_801e46b4_slot0b[ref_other.p->side], s2, r & 0xffff, ref_other.p->side);
}
