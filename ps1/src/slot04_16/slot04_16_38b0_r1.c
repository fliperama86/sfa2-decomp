/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void update_launch_attack_by_step_flag_table_slot04_16(Object *obj) {
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
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        game_state.other_object = obj->other;
        game_state.other_object->field_15b = 1;
        set_other_hit_reaction_state_0(obj, ((u8 *)b)[obj->direction_index & 0xfe], 0xf, -0x200, 0, 0, 1);
        game_state.other_object = obj->other;
        if ((s16)game_state.other_object->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        obj->vel_x = c[(obj->direction_index >> 1) * 4 + 0];
        obj->vel_y = c[(obj->direction_index >> 1) * 4 + 1];
        obj->accel_y = c[(obj->direction_index >> 1) * 4 + 3];
        obj->accel_x = 0;
        if (obj->facing_side != 0) {
            obj->vel_x = -obj->vel_x;
        }
    } else {
        obj->step_flags = t & 0xff00;
        game_state.config->setup_value = (obj->direction_index << 2) + 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        k = obj->field_49 != 0 ? 6 : 0; p = (s16 *)((obj->direction_index + k) * 2 + (u32)a);
        if ((u8)apply_damage_to_other_return_defeated(obj, p[0], p[1]) != 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
                game_state.config->countdown = 0;
                spawn_game_state_kind_object(obj);
            }
        }
        play_sound_at(obj, obj->side, 0x319);
    }
}
