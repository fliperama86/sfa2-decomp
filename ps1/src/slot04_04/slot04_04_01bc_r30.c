/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c4390_slot04_04[];

void func_801b27ac_slot04_04(Object *o) {
    o->state_3++;
    o->field_46 = ((Slot04bObj *)o)->field_46 + 0x400;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    if (o->field_0b != 0) {
        o->pos_x = o->pos_x + 0x18;
    } else {
        o->pos_x = o->pos_x - 0x18;
    }
    o->vel_x = data_801c4390_slot04_04[o->field_12a * 2];
    o->accel_x = data_801c4390_slot04_04[o->field_12a * 2 + 1];
    o->vel_y = data_801c4390_slot04_04[o->field_12a * 2 + 2];
    o->accel_y = data_801c4390_slot04_04[o->field_12a * 2 + 3];
    func_801307e0(o, o->field_12a + 0x53);
}
