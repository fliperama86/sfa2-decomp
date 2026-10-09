/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800197d8_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_48 = 0;
        obj->field_05++;
    }
}

void func_80019800_slot28(Object *obj) {
    *(s32 *)&obj->field_10 += 0xffff8000;
    if (obj->pos_x <= 0xc0) {
        obj->pos_x = 0xc0;
        obj->field_05++;
    }
}

void func_8001983c_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_48 = 0;
        obj->field_05++;
        func_80130768(obj, 7, (SequenceStep **)obj->box_tables);
    }
}

void func_8001987c_slot28(Object *obj) {
    if (obj->pos_x - obj->field_3c->pos_x >= 0x30) {
        obj->field_05++;
        func_80130768(obj, 8, (SequenceStep **)obj->box_tables);
    }
}
