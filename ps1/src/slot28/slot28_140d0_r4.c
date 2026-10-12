/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec4ab50 data_8004ab50_slot28[];
extern u8 data_8004ab80_slot28[];

void init_move_toward_table_target_slot28(Object *o) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Slot28Obj *obj = (Slot28Obj *)o;
    int dx, dy;
    obj->pos_x = 0xb0 << 16;
    obj->pos_y = 0x60 << 16;
    *(u16 *)&obj->countdown_timer = 0x10;
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->counter += 1;
    dx = (int)(data_8004ab50_slot28[obj->table_index].target_x - obj->pos_x) >> 4;
    dy = (int)(data_8004ab50_slot28[obj->table_index].target_y - obj->pos_y) >> 4;
    obj->delta_x = dx;
    obj->delta_y = dy;
    set_sequence_from_table_5(o, data_8004ab80_slot28[obj->table_index], obj->sequence_ptr);
}
