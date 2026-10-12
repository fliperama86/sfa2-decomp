/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 *data_801c2e14_slot04_0b[];
extern u8 *data_801c2e4c_slot04_0b[];
extern u8 *data_801c2e90_slot04_0b[];

void select_box_data_pointer_by_health_slot04_0b(Object *o) {
    u8 *v;

    if (o->vel_table_index == 0xc) {
        ref_first.p = (Object *)data_801c2e14_slot04_0b;
    } else if (o->vel_table_index == 0xe) {
        ref_first.p = (Object *)data_801c2e4c_slot04_0b;
    } else {
        ref_first.p = (Object *)data_801c2e90_slot04_0b;
    }
    v = ((u8 **)ref_first.p)[(s16)o->health];
    ptr_8019040c = (Box32 *)v;
    ((Slot04aObj *)o)->data_pointer = v;
}
