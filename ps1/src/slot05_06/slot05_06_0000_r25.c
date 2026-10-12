/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801dd37c_slot05_06[];
extern ObjectFn data_801dd388_slot05_06[];

void set_vel_x_from_direction_table_slot05_06(Object *obj) {
    obj->vel_x = data_801dd37c_slot05_06[obj->direction_index >> 1];
}

void func_801ca070_slot05_06(Object *obj) {
    data_801dd388_slot05_06[obj->state_3](obj);
}
