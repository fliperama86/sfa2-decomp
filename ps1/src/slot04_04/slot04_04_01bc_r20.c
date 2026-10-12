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
    func_80142c04(obj);
    func_80138ae8(&game_state, obj);
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
        if (!(u8)func_8014a170(obj, data_801c4224_slot04_04)) {
            update_sequence(obj);
            return;
        }
    }
tail:
    obj->state_3++;
    obj->active_flag_4 = 0;
    func_801307e0(obj, obj->direction_index + 0x22);
}
