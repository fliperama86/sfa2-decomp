/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern ObjectFn data_80022e20_slot12[];
void set_sequence_and_copy_to_shared_object_slot12(Object *obj, int a);
void add_tile_map_quads_for_frame_slot12(Object *obj, FrameRecord *f);

void func_8001201c_slot12(Object *obj) {
    Object *p;
    FrameRecord *f;

    data_80022e20_slot12[obj->state_1](obj);
    advance_sequence_checked(obj);
    data_8002d56c_slot12->sequence = obj->sequence;
    p = data_8002d56c_slot12;
    if (p->side != 0) {
        f = frames_right;
    } else {
        f = frames_left;
    }
    add_tile_map_quads_for_frame_slot12(p, f);
    set_flag_and_push_if_mode_8(obj);
}
