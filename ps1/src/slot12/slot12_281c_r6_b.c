/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801ae02d;
extern u8 data_801ae02e;
extern void (*data_80023170_slot12[])(Object *);
void func_8001281c_slot12(Object *obj);

void func_80012e2c_slot12(Object *obj) {
    data_80023170_slot12[obj->field_04](obj);
}

void func_80012e6c_slot12(Object *obj) {
    obj->field_0c = 1;
    obj->field_48 = 9;
    obj->pos_x = 0xf8;
    obj->field_01 = 0;
    obj->field_09 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_24 = 0;
    obj->pos_y = 0x80;
    obj->field_04++;
}
