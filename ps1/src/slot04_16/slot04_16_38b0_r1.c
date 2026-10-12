/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void func_801b38b0_slot04_16(Object *obj) {
    s16 a[12] = { 0x17, 0, 0x1b, 0, 0x1f, 0x12, 0x14, 0xf, 0x16, 0xf, 0x18, 0xf };
    s16 b[3] = { 0x20, 0x21, 0x22 };
    s32 c[12] = { 0x38000, 0x60000, 0, -0x8000, 0x30000, 0x70000, 0, -0x8400, 0x48000, 0x80000, 0, -0x8800 };
    u16 t;
    s16 *p;
    int k;
    u8 i;

    t = obj->step_flags;
    if ((u8)t == 0) {
        update_sequence(obj);
        return;
    }
    if ((u8)t != 0xf) {
        obj->field_45 = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        game_state.other_object = obj->other;
        game_state.other_object->field_15b = 1;
        func_80140770(obj, ((u8 *)b)[obj->field_12a & 0xfe], 0xf, -0x200, 0, 0, 1);
        game_state.other_object = obj->other;
        if ((s16)game_state.other_object->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        obj->vel_x = c[(obj->field_12a >> 1) * 4 + 0];
        obj->vel_y = c[(obj->field_12a >> 1) * 4 + 1];
        obj->accel_y = c[(obj->field_12a >> 1) * 4 + 3];
        obj->accel_x = 0;
        if (obj->field_0b != 0) {
            obj->vel_x = -obj->vel_x;
        }
    } else {
        obj->step_flags = t & 0xff00;
        game_state.config->field_63 = (obj->field_12a << 2) + 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        k = obj->field_49 != 0 ? 6 : 0; p = (s16 *)((obj->field_12a + k) * 2 + (u32)a);
        if ((u8)func_80140cd8(obj, p[0], p[1]) != 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
                game_state.config->field_6b = 0;
                func_80147000(obj);
            }
        }
        play_sound_at(obj, obj->side, 0x319);
    }
}
