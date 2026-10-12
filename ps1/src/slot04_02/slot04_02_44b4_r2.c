/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c64e8_slot04_02[];

void func_801b482c_slot04_02(Object *obj);
void func_801b494c_slot04_02(Object *obj);

void func_801b46d4_slot04_02(Object *obj) {
    u16 a;

    update_sequence(obj);
    if (obj->field_0b != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= obj->ground_y) {
        obj->field_157 = 1;
        obj->field_159 = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->field_45 = 0;
        obj->pos_y = obj->ground_y;
        func_801209c4(obj);
        a = obj->field_12a >> 1;
        obj->accel_x = data_801c64e8_slot04_02[a];
        if (obj->field_49 != 0 && obj->field_cd == 0) {
            a += 0x6d;
        } else {
            a += 0x4d;
        }
        func_801307e0(obj, a);
        play_sound_at(obj, obj->side, 0x324);
    } else if (obj->field_cd != 0) {
        func_801b494c_slot04_02(obj);
    } else {
        func_801b482c_slot04_02(obj);
    }
}
