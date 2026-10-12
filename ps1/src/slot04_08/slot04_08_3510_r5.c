/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3da8_slot04_08[];

void func_801b3bb8_slot04_08(Object *obj) {
    s16 t = obj->step_flags;
    s16 a;
    s16 n;
    int i;

    if (t & 0x8000) {
        n = obj->countdown;
        n--;
        obj->countdown = n;
        if (n != 0) {
            set_sequence_by_side_and_mode(obj, 0x5e);
        } else {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x5f);
        }
    } else {
        if (t & 0xff) {
            obj->step_flags = t & ~0xff;
            game_state.state_param = 0x18;
            i = obj->direction_index >> 1;
            if (obj->field_49 != 0) {
                i += 3;
            }
            a = data_801c3da8_slot04_08[i];
            if ((s16)obj->countdown != 1) {
                a |= ~0xff;
            }
            apply_damage_to_other_return_defeated(obj, a, 0);
            if ((s16)obj->other->health < 0) {
                play_sound_at(obj, obj->side, 0x319);
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x18;
                    obj->field_255 = 6;
                    game_state.spawn_kind = 0;
                    spawn_game_state_kind_object(obj);
                }
            } else {
                play_sound_at(obj, obj->side, 0x319);
            }
            func_80146960(obj);
        }
        update_sequence(obj);
    }
}
