/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e2fdc_slot2a[];
extern u32 data_801e515c_slot2a;
extern void (*data_801e2300_slot2a[])(Object *);
extern void (*data_801e230c_slot2a[])(Object *);
void func_801e12fc_slot2a(Object *obj);
void func_801e16a8_slot2a(Object *obj);

/* Residual: loop-invariant constants are hoisted as lui 0xff00 first, then lui 0xff / ori 0xffff; the original has them in the other order. */
void func_801e1244_slot2a(Object *obj) {
    data_801e2300_slot2a[obj->field_04](obj);
}

void func_801e1284_slot2a(Object *obj) {
    obj->field_09 = 1;
    obj->pos_x = 0x68;
    obj->field_01 = 0;
    obj->field_5c = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_24 = 0;
    obj->field_0c = 0;
    obj->field_04++;
    obj->field_05++;
    if (obj->field_45 != obj->field_03) {
        obj->pos_x = 0x10;
    }
    func_801e12fc_slot2a(obj);
}
