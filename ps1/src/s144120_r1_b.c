/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_80144220(Object *object) {
    *(s32 *)&object->pos_x_frac += object->vel_x;
    object->vel_x = object->vel_x + object->accel_x;
}

void func_80144244(Object *object) {
    *(s32 *)&object->pos_x_frac += object->vel_x;
    *(s32 *)&object->pos_y_frac -= object->vel_y;
}

void func_80144268(Object *object) {
    handlers_b5f4[object->state_2](object);
    advance_sequence_checked(object);
    func_80120028(object);
}

void func_801442c4(Object *object) {
    object->pos_x = -0x40;
    object->pos_y = 0x70;
    object->field_46 = 0xf;
    object->vel_x = 0xc0000;
    object->accel_x = 0x8000;
    object->state_2 = object->state_2 + 1;
    func_80130768(object, 8, table_8017b50c);
}
