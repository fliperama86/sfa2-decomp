/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80055eb2_slot01[];
extern SequenceStep *data_800242a8_slot01[];
extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];

void func_800130a8_slot01(Object *obj) {
    obj->field_01 = 1;
    obj->field_0e = 1;
    obj->field_09 = 0;
    obj->field_0b = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_0f = 0;
    obj->field_26 = 0;
    obj->field_0a = 1;
    obj->field_04++;
    obj->pos_x = box_margin[0];
    obj->pos_y = 0x2c;
    obj->field_90 = (void *)0x80059000;
    obj->field_98 = data_80019330_slot01;
    obj->field_9c = data_800195b8_slot01;
    func_80130768(obj, obj->field_03, data_800242a8_slot01);
    data_80055eb2_slot01[0] = 0x19d;
    data_80055eb2_slot01[2] = 0x20;
}

void func_80013154_slot01(Object *obj) {
    s16 x = obj->pos_x;
    s16 m = *(s16 *)box_margin;

    if (x < m + 0x153) {
        obj->pos_x = x + 8;
        if (*(s16 *)data_80055eb2_slot01 > 0x40) {
            *(s16 *)data_80055eb2_slot01 -= 8;
        }
    } else {
        obj->pos_x = m + 0x153;
    }
    func_8011ffdc(obj);
}
