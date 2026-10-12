/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec42274 data_80042274_slot28[];

void func_8002026c_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];

    obj->state_0++;
    obj->field_0e = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->vel_x = data_80042274_slot28[obj->table_index].vel_x;
    obj->vel_y = data_80042274_slot28[obj->table_index].vel_y;
    obj->accel_x = data_80042274_slot28[obj->table_index].accel_x;
    obj->accel_y = data_80042274_slot28[obj->table_index].accel_y;
    obj->field_0c = 0xff;
    if (obj->table_index != 0) {
        obj->image_row = 0;
        obj->facing_side = get_random_byte() & 1;
        func_80130768(obj, 2, (SequenceStep **)obj->box_tables);
    } else {
        obj->facing_side = 1;
        obj->image_row = 0;
        func_80130768(obj, 0, (SequenceStep **)obj->box_tables);
    }
}
