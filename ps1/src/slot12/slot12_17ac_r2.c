/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12List data_80022dc4_slot12;
extern u8 data_800175e0_slot12[];
extern u8 data_80017630_slot12[];
extern SequenceStep *table_80017870_slot12[];
void func_80011a78_slot12(Object *obj, Slot12List *list);
void func_80011bd8_slot12(Object *obj, Slot12List *list);

void func_800117ec_slot12(Object *obj) {
    obj->field_0e = 0;
    obj->field_24 = 0;
    obj->field_0b = 0;
    obj->field_04++;
    if (obj->field_03 == 0) {
        obj->field_20 = 0x80;
        obj->field_22 = 0x80;
        obj->pos_x = 0xb8;
        obj->pos_y = 0x60;
        obj->field_01 = 0;
        obj->field_0c = 0;
        obj->field_7a = 0;
        obj->field_7c = 0x1e2;
        func_80011a78_slot12(obj, &data_80022dc4_slot12);
        func_80011bd8_slot12(obj, &data_80022dc4_slot12);
    } else {
        obj->field_01 = 1;
        obj->field_0c = 1;
        obj->pos_x = 0xc0;
        obj->pos_y = 0x98;
        obj->field_7c = 0x1e3;
        obj->field_90 = (void *)0x80040000;
        obj->field_98 = data_800175e0_slot12;
        obj->field_09 = 0;
        obj->field_20 = 0;
        obj->field_7a = 0;
        obj->field_9c = data_80017630_slot12;
        func_80130768(obj, 3, table_80017870_slot12);
    }
}
