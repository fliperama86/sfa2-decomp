/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80148d48(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_0 = object->state_0 + 1;
    } else {
        s32 *wobble = (s32 *)data_8019045c;

        object->field_46 = object->field_46 + 1;
        *(s32 *)&object->pos_y_frac = *(s32 *)&object->pos_y_frac - object->vel_y;
        object->vel_y = object->vel_y + object->accel_y;
        *(s32 *)&object->pos_x_frac = object->vel_x;
        object->accel_x = object->accel_x + 0xc000;
        *wobble = (object->accel_x & 0xffff0000) | (s16)object->field_46;
        *(u8 *)wobble = *(u8 *)wobble + game_state.field_24;
        if ((*(u8 *)wobble & 1) != 0) {
            *wobble = -*wobble;
        }
        *(s32 *)&object->pos_x_frac = *(s32 *)&object->pos_x_frac + *wobble;
        advance_sequence_checked(object);
        func_80120028(object);
    }
}
