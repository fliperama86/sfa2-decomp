/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern int data_801c0854_slot04_0a[];

void set_vel_x_from_table_by_facing_slot04_0a(Object *obj) {
    int v = data_801c0854_slot04_0a[obj->vel_table_index >> 1];

    if (obj->facing_side == 0) {
        obj->vel_x = -v;
    } else {
        obj->vel_x = v;
    }
}

