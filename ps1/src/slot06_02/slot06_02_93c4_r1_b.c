/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801edf18_slot06_02[];
extern Slot06TileW data_801f8638_slot06_02[2][13];
extern u16 data_801e9e7c_slot06_02[];
extern s16 data_801e9e7e_slot06_02[];

void func_80136d70(Tx *tx);
void func_801e9604_slot06_02(Object *obj);

void func_801e9604_slot06_02(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u16 unused[4];

    obj->field_48 = data_801e9e7c_slot06_02[obj->field_88];
    obj->field_30 = data_801e9e7e_slot06_02[obj->field_88];
}

void func_801e9654_slot06_02(Slot06Layer *layer) {
    layer->field_50 = data_801edf18_slot06_02;
    layer->field_54 = data_801edf18_slot06_02;
    layer->field_1e = 0x6410;
    layer->field_58 = 0x200;
    layer->field_5c = 0x100;
}

void func_801e9680_slot06_02(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 13; i++) {
            p = data_801f8638_slot06_02[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
