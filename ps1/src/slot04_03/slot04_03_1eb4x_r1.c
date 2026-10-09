/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b223c_slot04_03(Object *obj);

extern Slot04_03Rec0678 data_801c0678_slot04_03[];

void func_801b1eb4_slot04_03(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[0x18];
    s16 t;
    Object *o;

    func_80130efc(obj);
    t = obj->field_3a;
    if (!(t & 0x8000)) {
        if ((u8)t == 0) {
            func_801b223c_slot04_03(obj);
        }
    } else {
        obj->field_07++;
        func_801204f4(obj, obj->side, 5);
        func_801204f4(obj, obj->side, 0xd);
        obj->field_4c = data_801c0678_slot04_03[obj->field_12a >> 1].field_00;
        o = obj;
        o->field_54 = data_801c0678_slot04_03[obj->field_12a >> 1].field_04;
        o->field_50 = data_801c0678_slot04_03[obj->field_12a >> 1].field_08;
        o->field_58 = data_801c0678_slot04_03[o->field_12a >> 1].field_0c;
        t = (o->field_12a >> 1) + 0x35;
        func_801307e0(o, t);
    }
}
