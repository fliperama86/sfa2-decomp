/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801c4224_slot04_04[];

void func_801b1a54_slot04_04(Object *obj) {
    u32 *w = (u32 *)&game_state.leader_bits;
    u16 t;

    if ((*w & 0xffff00) != 0 || game_state.field_04 != 0 || (u8)func_8012f56c(obj)) {
        goto tail;
    }
    t = obj->step_flags;
    if ((t & 0x7f00) == 0 || (t & 0x7f00) != 0x100) {
        update_sequence(obj);
        return;
    }
    obj->step_flags = t & 0x80ff;
    reset_hold_flags_and_other_hit_count(obj);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->input_matched_flag == 0) {
        obj->input_matched_flag = 0xff;
    }
    if (obj->input_dir_second != 0) {
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown != 0) {
            update_sequence(obj);
            return;
        }
    } else {
        if (!(u8)test_random_bit_in_table(obj, data_801c4224_slot04_04)) {
            update_sequence(obj);
            return;
        }
    }
tail:
    obj->state_3++;
    obj->active_flag_4 = 0;
    set_sequence_by_side_and_mode(obj, obj->direction_index + 0x22);
}
