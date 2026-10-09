/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern Pair data_80015c8c_slot01[];
extern Pair data_80015c10_slot01[];
extern s16 data_80015c60_slot01[];
extern SequenceStep *data_80023ee0_slot01[];
extern SequenceStep *data_80023e48_slot01[];
extern Slot01Rec152ec data_80015bc8_slot01[];
extern u16 data_80055f30_slot01;
extern u16 data_80055f34_slot01;
void func_80011fe8_slot01(Object *obj, Slot01Rec152ec *p);

void func_80013c24_slot01(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    obj->field_01 = 1;
    obj->field_09 = 9;
    obj->field_98 = data_80019330_slot01;
    obj->field_9c = data_800195b8_slot01;
    obj->field_0c = 1;
    obj->field_0b = 0;
    obj->field_0d = 0;
    obj->field_0f = 0;
    obj->field_90 = (void *)0x80059000;
    obj->field_04++;
    if (obj->field_60 != 0) {
        Object *p = obj;
        p->pos_x = data_80015c8c_slot01[p->field_03].first + data_80055f30_slot01;
        p->pos_y = -data_80015c8c_slot01[p->field_03].second + data_80055f34_slot01 + 0x200;
        func_80130768(p, 0, data_80023ee0_slot01);
    } else {
        obj->pos_x = data_80015c10_slot01[obj->field_03].first + data_80055f30_slot01;
        obj->pos_y = -data_80015c10_slot01[obj->field_03].second + data_80055f34_slot01 + 0x230;
        func_80130768(obj, data_80015c60_slot01[obj->field_03], data_80023e48_slot01);
        func_80011fe8_slot01(obj, data_80015bc8_slot01);
    }
}
