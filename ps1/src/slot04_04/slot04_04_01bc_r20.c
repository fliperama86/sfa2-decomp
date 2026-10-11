/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801c4224_slot04_04[];

void func_801b1a54_slot04_04(Object *obj) {
    u32 *w = (u32 *)&game_state.field_4c;
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
    if (obj->field_254 == 0) {
        obj->field_254 = 0xff;
    }
    if (obj->field_129 != 0) {
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 != 0) {
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
    obj->field_17b = 0;
    func_801307e0(obj, obj->field_12a + 0x22);
}
