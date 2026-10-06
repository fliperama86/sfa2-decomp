/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80022e80_slot12[])(Object *);
extern void (*data_80022e8c_slot12[])(Object *);
void func_80012b5c_slot12(Object *obj);
void func_80012a0c_slot12(Object *obj);

void func_8001281c_slot12(Object *obj) {
    if (obj->field_03 & 0x80) {
        data_80022e8c_slot12[obj->field_04](obj);
    } else {
        data_80022e80_slot12[obj->field_04](obj);
        func_80012b5c_slot12(obj);
    }
}

void func_800128b4_slot12(Object *obj) {
    obj->field_46 = 0x30;
    obj->pos_x = 0xc0;
    *(s32 *)&obj->field_14 = 0;
    obj->pos_y = -0x70;
    obj->field_58 = 0x1800;
    obj->field_01 = 0;
    obj->field_0e = 0;
    obj->field_24 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_0c = 0;
    obj->field_0b = 0;
    obj->field_09 = 0;
    obj->field_50 = 0;
    obj->field_04++;
    if (obj->field_03 != 0) {
        obj->pos_y = 0xe0;
        obj->field_58 = -obj->field_58;
    }
    func_80012a0c_slot12(obj);
}
