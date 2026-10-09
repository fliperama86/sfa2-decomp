/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c1630_slot04_05[];
extern int data_801c1700_slot04_05;

void func_801b39e0_slot04_05(Object *obj, int a1, int a2);
void func_801b3a70_slot04_05(Object *obj, int a1, int a2);

int func_801b3984_slot04_05(Object *obj, int a1, int a2) {
    u8 i = a1;

    data_801c1700_slot04_05 = 0;
    if (obj->slots[i].field_00 == 0) {
        func_801b39e0_slot04_05(obj, i, (u8)a2);
    } else {
        func_801b3a70_slot04_05(obj, i, (u8)a2);
    }
    return data_801c1700_slot04_05;
}

void func_801b39e0_slot04_05(Object *obj, int a1, int a2) {
    u8 k = a2;

    if (obj->field_134 & data_801c1630_slot04_05[k * 3]) {
        obj->slots[(u8)a1].field_00++;
        ((u8 *)&obj->slots[(u8)a1])[2] = ((u8 *)&data_801c1630_slot04_05[k * 3])[2];
        obj->slots[(u8)a1].field_04 = ((u8 *)&data_801c1630_slot04_05[k * 3])[4];
    } else {
        obj->slots[(u8)a1].field_00 = 0;
    }
}
