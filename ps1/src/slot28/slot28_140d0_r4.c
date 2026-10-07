/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec4ab50 data_8004ab50_slot28[];
extern u8 data_8004ab80_slot28[];

void func_800245dc_slot28(Object *o) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Slot28Obj *obj = (Slot28Obj *)o;
    int dx, dy;
    obj->field_10 = 0xb0 << 16;
    obj->field_14 = 0x60 << 16;
    *(u16 *)&obj->field_46 = 0x10;
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04 += 1;
    dx = (int)(data_8004ab50_slot28[obj->field_03].field_00 - obj->field_10) >> 4;
    dy = (int)(data_8004ab50_slot28[obj->field_03].field_04 - obj->field_14) >> 4;
    obj->field_4c = dx;
    obj->field_50 = dy;
    func_80130768(o, data_8004ab80_slot28[obj->field_03], obj->field_6c);
}
