/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec4aac4 data_8004aac4_slot28[];
extern u8 data_8004aab8_slot28[];
extern u8 data_8004aac0_slot28[];

void init_motion_from_table_and_set_sequence_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];
    obj->field_0e = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->vel_x = data_8004aac4_slot28[obj->table_index].vel_x;
    obj->accel_x = data_8004aac4_slot28[obj->table_index].accel_x;
    obj->vel_y = data_8004aac4_slot28[obj->table_index].vel_y;
    obj->accel_y = data_8004aac4_slot28[obj->table_index].accel_y;
    if (obj->table_index != 0) {
        set_sequence_from_table_5(obj, data_8004aab8_slot28[obj->table_index], ((Slot28Obj *)obj)->sequence_ptr);
    } else {
        set_sequence_from_table_5(obj, data_8004aac0_slot28[obj->variant_index_2], ((Slot28Obj *)obj)->sequence_ptr);
    }
}
