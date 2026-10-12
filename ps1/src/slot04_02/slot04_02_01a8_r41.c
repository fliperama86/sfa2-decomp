/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c650c_slot04_02[];

void func_801b5090_slot04_02(Object *obj) {
    update_sequence(obj);
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_49 != 0) {
            obj->active_flag_4 = 0;
            obj->active_state = 0;
        }
        obj->vel_x = 0x20000;
        obj->pos_y = obj->ground_y;
        obj->accel_x = data_801c650c_slot04_02[obj->direction_index >> 1];
        func_801307e0(obj, 0x55);
        play_sound_at(obj, obj->side, 0x324);
    }
}
