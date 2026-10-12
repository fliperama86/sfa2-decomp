/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"


extern Object *data_8002d56c_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern ObjectFn data_80022e20_slot12[];
void set_sequence_and_copy_to_shared_object_slot12(Object *obj, int a);
void add_tile_map_quads_for_frame_slot12(Object *obj, FrameRecord *f);

void select_state_1_and_sequence_from_flags_slot12(Object *obj) {
    if (game_state.field_44 != 0 && data_8002d578_slot12 == 0) {
        obj->state_1 = 3;
        set_sequence_and_copy_to_shared_object_slot12(obj, 1);
    } else if (game_state.status_flags & 0x80) {
        obj->state_1 = obj->state_1 + 1;
        set_sequence_and_copy_to_shared_object_slot12(obj, 2);
    }
}
