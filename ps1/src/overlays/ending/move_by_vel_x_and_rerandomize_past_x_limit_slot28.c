/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8004ea70_slot28[];
extern int data_8004ea74_slot28[];
void randomize_position_velocity_and_sequence_2_slot28(Object *obj);

void move_by_vel_x_and_rerandomize_past_x_limit_slot28(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    if (obj->pos_x >= 0x140) {
        randomize_position_velocity_and_sequence_2_slot28(obj);
    }
}

void randomize_position_velocity_and_sequence_2_slot28(Object *obj) {
    int r = get_random_byte() & 0xff;
    int k = data_8004ea70_slot28[r & 3];
    int row = k - 6;
    obj->pos_x = r & 0x3f;
    obj->pos_y = 0x98 - (r & 0x7f);
    obj->vel_x = data_8004ea74_slot28[row * 8 + (r & 7)];
    set_sequence_from_table_5(obj, k, (SequenceStep **)obj->box_tables);
}
