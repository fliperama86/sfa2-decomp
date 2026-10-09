/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4874_slot04_06(Object *obj, u16 *value);

void func_801b4cfc_slot04_06(Object *obj) {
    u16 x;
    s8 r;

    if (((Slot04aObj *)obj)->field_3b & 0x80) {
        game_state_second.field_4b = game_state.field_4b | (1 << obj->side);
    }
    if (((Slot04aObj *)obj)->field_3a == 1) {
        r = func_80146918(obj);
        if (r) {
            x = 7;
            func_801b4874_slot04_06(obj, &x);
            ref_other.p->pos_x = ref_other.p->pos_x + x;
            ref_other.p->pos_y = ref_other.p->pos_y - 0x47;
            ref_other.p->field_44 = 1;
        }
    }
    func_80130efc(obj);
}
