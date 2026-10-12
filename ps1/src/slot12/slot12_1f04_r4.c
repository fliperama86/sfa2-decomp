/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
void init_polyft4_cell_arrays_slot12(void);
void play_sound_and_decrement_counter_slot12(int a);

void copy_object_state_to_shared_object_slot12(Object *obj) {
    data_8002d56c_slot12->field_02 = 0;
    data_8002d56c_slot12->sequence = obj->sequence;
    data_8002d56c_slot12->pos_x = obj->pos_x;
    data_8002d56c_slot12->pos_y = obj->pos_y;
    data_8002d56c_slot12->facing_side = obj->facing_side;
    data_8002d56c_slot12->spawn_x = obj->spawn_x;
    data_8002d56c_slot12->spawn_y = obj->spawn_y;
    data_8002d56c_slot12->image_row = 0;
    init_polyft4_cell_arrays_slot12();
}

void consume_step_flags_low_byte_and_play_sound_slot12(Object *obj) {
    u8 v;

    if (!(game_state.field_44 & 0x80)) {
        v = obj->step_flags;
        if (v != 0) {
            obj->step_flags = obj->step_flags & 0xff00;
            play_sound_and_decrement_counter_slot12(v);
        }
    }
}
