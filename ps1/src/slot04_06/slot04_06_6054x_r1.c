#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c575c_slot04_06;
extern void (*data_801c55d4_slot04_06[])(Object *);

void func_801b6054_slot04_06(Object *obj) {
    data_801c575c_slot04_06 = obj->field_3c;
    data_801c55d4_slot04_06[obj->field_04](obj);
}
