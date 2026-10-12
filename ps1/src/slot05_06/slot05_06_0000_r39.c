/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void negate_value_if_facing_side_zero_slot05_06(Object *obj, u16 *out);
extern s32 data_801dd45c_slot05_06[];

void start_move_from_table_with_hit_check_slot05_06(Object *obj) {
    s16 d;
    s32 *e;

    obj->state_3++;
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    if ((s8)spawn_random_offset_effect_index_two(obj)) {
        d = -0x1b;
        negate_value_if_facing_side_zero_slot05_06(obj, (u16 *)&d);
        ref_other.p->pos_x += d;
        ref_other.p->pos_y = ref_other.p->pos_y;
    }
    e = &data_801dd45c_slot05_06[obj->direction_index];
    obj->vel_x = e[0];
    obj->accel_x = e[1];
    obj->active_flag = 0;
    play_side_sound_at(obj, obj->side, 9);
    set_sequence_by_side_and_mode(obj, 0x2b);
}
