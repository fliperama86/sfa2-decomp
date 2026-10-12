/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b25a8_slot04_08(Object *object);

void func_801b249c_slot04_08(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->ground_y < obj->pos_y) {
        obj->pos_y = obj->ground_y;
        obj->field_45 = 0;
        func_801209c4(obj);
        if ((u8)func_8013f8c4(obj, -0x20, 0x30) != 0) {
            obj->state_3++;
            obj->field_46 = 0;
            play_sound_at(obj, obj->side ^ 1, 0x31a);
            func_80145f98(obj);
            play_side_sound_at(obj, obj->side, 0x12);
            func_801b25a8_slot04_08(obj);
        } else {
            obj->state_3 = 9;
            func_801307e0(obj, 0x33);
        }
    } else {
        update_sequence(obj);
    }
}
