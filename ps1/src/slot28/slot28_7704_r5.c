/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80017ed0_slot28(Object *obj, Object *src) {
    obj->field_00 = 1;
    obj->field_02 = 0xa5;
    obj->field_03 = 0;
    obj->field_01 = 1;
    obj->field_7a = 0x60;
    obj->field_7c = 0x1e0;
    obj->field_0d = 0;
    obj->field_90 = src->field_90;
    obj->field_98 = src->field_98;
    obj->field_9c = src->field_9c;
    obj->box_tables = src->box_tables;
}
