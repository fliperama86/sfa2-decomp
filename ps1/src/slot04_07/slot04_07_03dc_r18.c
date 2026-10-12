/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c20e8_slot04_07[])(Object *, Object *);
void func_801b42a4_slot04_07(Object *obj, Object *parent);

int func_801b41c4_slot04_07(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    return obj->vel_y = obj->vel_y + obj->accel_y;
}

void func_801b4208_slot04_07(Object *obj) {
    data_801c20e8_slot04_07[obj->state_0](obj, obj->parent);
}

void func_801b4248_slot04_07(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    func_801b42a4_slot04_07(obj, parent);
}
