/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4508_slot04_04[];
extern s32 data_801c4520_slot04_04[];
extern u8 data_801c452c_slot04_04[];

void func_801b3c5c_slot04_04(Object *obj);

void func_801b3b40_slot04_04(Object *obj) {
    Object *parent;
    u8 i;
    int k;

    parent = obj->field_3c;
    i = obj->field_ac;
    i >>= 1;
    obj->field_09 = 0;
    obj->field_04 = obj->field_04 + 1;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04aObj *)obj)->field_6c = data_801c4508_slot04_04;
    obj->field_45 = 0;
    obj->field_50 = 0;
    k = data_801c4520_slot04_04[i];
    if (obj->field_0b != 0) {
        obj->field_4c = k;
        obj->pos_x = obj->pos_x + 0x58;
    } else {
        k = -k;
        obj->field_4c = k;
        obj->pos_x = obj->pos_x - 0x58;
    }
    obj->pos_y = obj->pos_y - 0x38;
    obj->field_a0 = data_801c452c_slot04_04[obj->field_ac >> 1];
    if (parent->side == 0) {
        ((Slot04aObj *)obj)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04aObj *)obj)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(obj, obj->field_ac >> 1);
    func_801b3c5c_slot04_04(obj);
}
