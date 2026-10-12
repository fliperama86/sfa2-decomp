/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4874_slot04_06(Object *obj, u16 *out);
extern s32 data_801c5460_slot04_06[];

void func_801b320c_slot04_06(Object *obj) {
    s16 d;
    s32 *e;

    obj->state_3++;
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    if ((s8)func_80146888(obj)) {
        d = -0x1b;
        func_801b4874_slot04_06(obj, (u16 *)&d);
        ref_other.p->pos_x += d;
        ref_other.p->pos_y = ref_other.p->pos_y;
    }
    e = &data_801c5460_slot04_06[obj->direction_index];
    obj->vel_x = e[0];
    obj->accel_x = e[1];
    obj->active_flag = 0;
    play_side_sound_at(obj, obj->side, 9);
    func_801307e0(obj, 0x2b);
}
