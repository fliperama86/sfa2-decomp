/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8003c2f4_slot28[];
extern int data_8003c2f8_slot28[];

void randomize_position_velocity_and_sequence_slot28(Object *obj);

void advance_state_0_on_negative_step_flags_slot28(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}

void move_by_vel_x_and_respawn_past_edge_slot28(Object *obj) {
    *(int *)&obj->pos_x_frac = *(int *)&obj->pos_x_frac + obj->vel_x;
    if (obj->pos_x >= 0x138) {
        randomize_position_velocity_and_sequence_slot28(obj);
    }
}

void randomize_position_velocity_and_sequence_slot28(Object *obj) {
    int v = get_random_byte();
    int a = v & 0x7f;
    int k = data_8003c2f4_slot28[v & 3];
    int j = k - 3;
    obj->pos_x = v & 0x3f;
    obj->pos_y = 0x98 - a;
    obj->vel_x = data_8003c2f8_slot28[(j << 3) + (v & 7)];
    set_sequence_from_table_5(obj, k, (SequenceStep **)obj->box_tables);
}
