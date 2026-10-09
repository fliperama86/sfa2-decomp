/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001c720_slot28(Object *o) {
    if (o->field_48 != 0) {
        o->field_48 = 0;
        o->field_05++;
        func_80130768(o, 9, ((Slot28Obj *)o)->field_6c);
    }
}

void func_8001c760_slot28(Object *o) {
    func_80131094(o);
    if (o->field_48 != 0) {
        o->field_48 = 0;
        o->field_09 = 4;
        o->field_05++;
        func_80130768(o, 5, ((Slot28Obj *)o)->field_6c);
    } else {
        int v = ((Slot28Obj *)o)->field_3a;
        o->field_09 = (v == 1) ? 4 : 2;
    }
}

void func_8001c7dc_slot28(Object *o) {
    if (o->field_48 != 0xff) {
        o->field_48 = 0;
        o->field_05++;
        func_80130768(o, 10, ((Slot28Obj *)o)->field_6c);
    }
}
