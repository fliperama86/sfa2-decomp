/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6518_slot04_02[];
extern u8 data_801c6520_slot04_02[];

void func_801b53ac_slot04_02(Object *obj) {
    int i;
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0 && obj->pos_y >= obj->field_70) {
        obj->field_45 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_80146960(obj);
        i = obj->field_12a & 0xfe;
        game_state.field_63 = data_801c6518_slot04_02[i];
        if ((u8)func_80140cd8(obj, *(s16 *)(data_801c6520_slot04_02 + i), 0) != 0) {
            play_sound_at(obj, obj->side, 0x319);
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x12;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            }
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
        func_801307e0(obj, 0x59);
    } else {
        update_sequence(obj);
    }
}
