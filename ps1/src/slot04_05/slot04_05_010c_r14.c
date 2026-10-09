/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c1648_slot04_05[])(Object *, Object *);

void func_801b3b14_slot04_05(Object *object) {
    u8 *p = (u8 *)object->slots;
    int i;
    u8 z = 0;

    for (i = 0x57; i >= 0; i--) {
        *p++ = z;
    }
}

void func_801b3b38_slot04_05(Object *obj) {
    data_801c1648_slot04_05[obj->field_04](obj, obj->field_3c);
}
