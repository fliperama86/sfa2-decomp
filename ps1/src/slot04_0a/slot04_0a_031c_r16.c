/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0864_slot04_0a[])(Object *, Object *);

void func_801b4368_slot04_0a(Object *obj) {
    data_801c0864_slot04_0a[obj->field_04](obj, obj->field_3c);
}
