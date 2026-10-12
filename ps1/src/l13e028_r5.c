/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* The seventh parameter is unused here. It is declared because a caller in a character module passes seven arguments; that is compatible with a parameter and does not prove one. */
void func_80140598(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, int unused) {
    ref_other.p = object->other;
    ref_other.p->state_0 = 1;
    ref_other.p->state_1 = 1;
    ref_other.p->state_2 = 1;
    ref_other.p->state_3 = 8;
    ref_other.p->vec_table_index = a;
    ref_other.p->field_60 = 2;
    ref_other.p->hit_type = b;
    ref_other.p->status_flag = 0;
    object->status_flag = 0;
    ref_other.p->field_261 = 0;
    ref_other.p->field_27a = 0;
    ref_other.p->facing_flag = object->facing_side;
    ref_other.p->facing_flag = e ^ ref_other.p->facing_flag;
    ref_other.p->field_243 = 1;
    object->field_292 = 0x1e;
    ref_other.p->field_292 = 0x1e;
    ref_other.p->countdown = 0x40;
    ref_other.p->hit_flag = 0xff;
    ref_other.p->field_17a = 0;
    if (game_state.versus_flag != 0 && game_state.mode == object->side + 1) {
        ref_other.p->field_19f = 0;
        ref_other.p->hit_count_2 = 0;
        ref_other.p->field_1a1 = 0;
        ref_other.p->field_1a2 = 0;
    }
    func_801409d8(object, c, d);
}
