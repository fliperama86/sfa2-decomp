/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b66fc_slot04_06(Object *obj);

void func_801b6440_slot04_06(Object *obj) {
    if ((func_801b66fc_slot04_06(obj) << 16) > 0) {
        obj->vel_x = 0x2000;
        if ((obj->facing_side ^ (obj->table_index & 1)) != 0) {
            obj->vel_x = -0x2000;
        }
        obj->vel_y = 0x1c000;
        obj->accel_y = -0x1800;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        obj->state_1++;
    }
    advance_sequence_checked(obj);
}
