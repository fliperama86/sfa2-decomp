/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c5e30_slot04_0f[];
extern s32 data_801c5e48_slot04_0f[];
extern u8 data_801c5e54_slot04_0f[];

void func_801b4ebc_slot04_0f(Object *obj);

void func_801b4da8_slot04_0f(Object *obj) {
    Object *parent;
    int dx;
    int i;

    parent = obj->field_3c;
    obj->field_09 = 0;
    obj->field_04 = obj->field_04 + 1;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->field_6c = data_801c5e30_slot04_0f;
    obj->field_45 = 0;
    obj->field_50 = 0;
    i = data_801c5e48_slot04_0f[obj->field_ac >> 1];
    dx = 0x53;
    if (obj->field_0b == 0) {
        i = -i;
        dx = -0x53;
    }
    obj->field_4c = i;
    obj->pos_x = obj->pos_x + dx;
    obj->pos_y = obj->pos_y - 0x39;
    obj->field_a0 = data_801c5e54_slot04_0f[obj->field_ac >> 1];
    if (parent->side == 0) {
        ((Slot04bObj *)obj)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(obj, (obj->field_ac >> 1) + 8);
    func_801b4ebc_slot04_0f(obj);
}
