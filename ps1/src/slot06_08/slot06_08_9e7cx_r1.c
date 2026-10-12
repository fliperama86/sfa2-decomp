/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed480_slot06_08[];

void init_object_state_and_start_sequence_6_or_7_slot06_08(Object *obj) {
    u8 t;
    u8 g;
    obj->spawn_mode = 4;
    g = obj->table_index;
    t = 1;
    obj->field_0c = 0;
    obj->field_0a = t;
    obj->field_0f = t;
    obj->state_0 = t;
    t = *(u8 *)&game_state.count_index;
    if (g != 0) t = ~t;
    if (t & 1) obj->state_0 = 2;
    t = 6;
    if (obj->table_index != 0) t = 7;
    set_sequence_entry(obj, data_801ed480_slot06_08[t]);
}
