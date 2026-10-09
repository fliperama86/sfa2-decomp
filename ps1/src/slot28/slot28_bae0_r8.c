/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003a190_slot28[])(Object *);

void func_8001c924_slot28(Object *object) {
    data_8003a190_slot28[object->field_05](object);
}

void func_8001c964_slot28(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t == 0) {
        obj->field_05++;
    }
}
