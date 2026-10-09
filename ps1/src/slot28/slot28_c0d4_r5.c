/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001c864_slot28(Object *o) {
    if (o->field_48 != 0) {
        o->field_05++;
        func_80130768(o, 11, ((Slot28Obj *)o)->field_6c);
    }
}

void func_8001c8a0_slot28(Object *o) {
    if (*(u8 *)&((Slot28Obj *)o)->field_3a != 0) {
        o->field_05++;
    }
    func_80131094(o);
}

void func_8001c8e0_slot28(Object *o) {
    if ((o->field_3a << 16) < 0) {
        o->field_04++;
    }
    func_80131094(o);
}
