/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801c3c54_slot04_08[];

void deal_table_damage_on_step_flag_byte_slot04_08(Object *obj) {
    u16 t;
    int i;
    int m = -0x100;
    t = obj->step_flags;
    if ((u8)t != 0) {
        if ((t & 0x80) == 0) {
            obj->step_flags = t & 0xff00;
            game_state.state_param = 0x18;
            spawn_two_effects_at_other_ground(obj);
            play_sound_at(obj, obj->side, 0x319);
        }
        obj->state_3++;
        i = obj->direction_index >> 1;
        if (obj->field_49 != 0) {
            i += 3;
        }
        apply_damage_to_other_return_defeated(obj, (s16)(data_801c3c54_slot04_08[i] | m), 0);
    }
    update_sequence(obj);
}
