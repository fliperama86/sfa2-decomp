/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80019640_slot28(Object *obj) {
    int t = *(u16 *)&obj->pos_y - 0x10;
    obj->pos_y = t;
    if ((s16)t < 0xa0) {
        obj->field_46 = 4;
        obj->pos_y = 0xa0;
        obj->field_05++;
    }
}

void func_80019684_slot28(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t == 0) {
        obj->field_46 = 0x20;
        obj->field_7c = 0x1e2;
        obj->field_05++;
    }
}

void func_800196c0_slot28(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t == 0) {
        obj->field_05++;
    }
}
