/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800197d8_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->variant_index = 0;
        obj->state_1++;
    }
}

void func_80019800_slot28(Object *obj) {
    *(s32 *)&obj->pos_x_frac += 0xffff8000;
    if (obj->pos_x <= 0xc0) {
        obj->pos_x = 0xc0;
        obj->state_1++;
    }
}

void func_8001983c_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->variant_index = 0;
        obj->state_1++;
        func_80130768(obj, 7, (SequenceStep **)obj->box_tables);
    }
}

void func_8001987c_slot28(Object *obj) {
    if (obj->pos_x - obj->parent->pos_x >= 0x30) {
        obj->state_1++;
        func_80130768(obj, 8, (SequenceStep **)obj->box_tables);
    }
}
