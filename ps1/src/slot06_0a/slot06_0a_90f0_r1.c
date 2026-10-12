/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed400_slot06_0a[];
extern ObjectFn data_801ed444_slot06_0a[];
extern void (*data_801ed454_slot06_0a[])(Object *, int, int);

void func_801e90f0_slot06_0a(Object *obj) {
    data_801ed444_slot06_0a[obj->state_0](obj);
}

void func_801e9130_slot06_0a(Object *obj) {
    obj->variant_index = 1;
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->field_0c = 0;
    obj->vel_x = *(s32 *)&obj->pos_x_frac;
    obj->vel_y = 0xf80000 - *(s32 *)&obj->pos_y_frac;
    func_80130700(obj, data_801ed400_slot06_0a[2 + obj->table_index]);
}

void func_801e91a8_slot06_0a(Object *obj) {
    Slot06Layer *l = (Slot06Layer *)data_801aa5d4;

    int x = *(s32 *)&l->bound_x_a - *(s32 *)&l->bound_x_c;
    int y = *(s32 *)&l->bound_y_a - *(s32 *)&l->bound_y_c;

    data_801ed454_slot06_0a[obj->table_index](obj, x, y);
    obj->variant_index = obj->variant_index ^ 1;
    if (obj->variant_index != 0) {
        func_8011ffdc(obj);
    } else {
        obj->field_01 = 0;
    }
}
