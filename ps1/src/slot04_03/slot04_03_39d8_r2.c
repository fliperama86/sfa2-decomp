/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c098c_slot04_03[];
extern s32 data_801c09a4_slot04_03[];
extern u16 data_801c09b0_slot04_03[];

void func_801b3c48_slot04_03(Object *obj, Object *unused);

/* The call of func_801b3c48_slot04_03 passes one argument although the function takes two: the original does not set the second argument register before it. Written with a second parameter passed on, this function is 12 bytes longer than the original. */
void func_801b3ae8_slot04_03(Object *obj, Object *unused) {
    u8 a2;
    int a1;

    obj->field_a0 = 0xff;
    obj->field_04++;
    obj->field_09 = 0;
    ref_other.p = obj->field_3c;
    obj->field_49 = ref_other.p->field_49;
    a2 = 3;
    obj->field_1c = ref_other.p->field_1c;
    ((Slot04aObj *)obj)->field_6c = data_801c098c_slot04_03;
    obj->field_45 = 0;
    obj->field_50 = 0;
    obj->field_4c = 0x50000;
    if (obj->field_03 == 0) {
        obj->field_4c = data_801c09a4_slot04_03[obj->field_ac >> 1];
        a2 = 0;
    }
    a1 = data_801c09b0_slot04_03[obj->field_70];
    if (obj->field_0b == 0) {
        a1 = -a1;
        obj->field_4c = -obj->field_4c;
    }
    if (ref_other.p->side == 0) {
        ((Slot04aObj *)obj)->field_8c = *(s32 *)0x1f8000a8;
    } else {
        ((Slot04aObj *)obj)->field_8c = *(s32 *)0x1f800158;
    }
    obj->pos_x = a1 + obj->pos_x;
    obj->pos_y = obj->pos_y - 0x44;
    func_80138070(obj, a2 + (obj->field_ac >> 1));
    ((void (*)(Object *))func_801b3c48_slot04_03)(obj);
}
