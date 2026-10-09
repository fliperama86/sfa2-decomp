/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80019510_slot28(Object *obj) {
    *(u16 *)&obj->pos_y += 0x10;
    if (obj->pos_y >= 0x61) {
        obj->field_46 = 4;
        obj->pos_y = 0x60;
        obj->field_05++;
    }
}

void func_80019554_slot28(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t == 0) {
        obj->field_46 = 4;
        obj->field_7c = 0x1e2;
        obj->field_05++;
    }
}

void func_80019590_slot28(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t == 0) {
        obj->field_6b = 0xff;
        obj->field_05++;
    }
}

void func_800195c4_slot28(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t != 0) {
        obj->field_05++;
    }
}
