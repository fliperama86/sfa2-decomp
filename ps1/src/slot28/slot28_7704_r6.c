/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80032a5c_slot28[])(Object *);

void func_80017f34_slot28(Object *object) {
    data_80032a5c_slot28[object->field_05](object);
}

void func_80017f74_slot28(Object *o) {
    ((Slot28Obj *)o)->field_01 = 0;
    if (((Slot28Obj *)o)->field_47 != 0) {
        ((Slot28Obj *)o)->field_46 --;
        if (((Slot28Obj *)o)->field_46 == 0) {
            ((Slot28Obj *)o)->field_01 = 1;
            ((Slot28Obj *)o)->field_47 --;
            ((Slot28Obj *)o)->field_46 = ((Slot28Obj *)o)->field_47;
        }
    } else {
        ((Slot28Obj *)o)->field_05 ++;
    }
}

void func_80017fd4_slot28(Object *obj) {
    obj->field_01 = 1;
}
