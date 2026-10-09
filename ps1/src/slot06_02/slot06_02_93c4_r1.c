/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eaf18_slot06_02[];
extern Poly28 data_801f5cf8_slot06_02[2][0x84];

void func_8015c09c(void *prim);
void func_801e94d8_slot06_02(Object *obj);
void func_801e9530_slot06_02(Object *obj);
void func_801e9604_slot06_02(Object *obj);

void func_801e93c4_slot06_02(Slot06Layer *layer) {
    layer->field_50 = data_801eaf18_slot06_02;
    layer->field_54 = data_801eaf18_slot06_02;
    layer->field_1e = 0x5816;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e93f0_slot06_02(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f5cf8_slot06_02[i][j];
            func_8015c09c(r);
            r->field_04 = c;
            r->field_05 = c;
            r->field_06 = c;
            j++;
        } while (j < 0x84);
        i++;
    } while (i < 2);
}

void func_801e9490_slot06_02(Object *obj) {
    if (obj->field_04 == 0) {
        func_801e94d8_slot06_02(obj);
    } else if (obj->field_04 == 1) {
        func_801e9530_slot06_02(obj);
    }
}

void func_801e94d8_slot06_02(Object *obj) {
    func_80136a2c((Cam *)obj, 0x40, 0);
    if (obj->field_01 != 0) {
        obj->field_01 = 0;
        ((Slot06Obj *)obj)->field_88 = 0;
        obj->field_04++;
        func_801e9604_slot06_02(obj);
    }
}
