#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c575c_slot04_06;
extern void (*data_801c55d4_slot04_06[])(Object *);

void call_handler_by_state_0_with_parent_2_slot04_06(Object *obj) {
    data_801c575c_slot04_06 = obj->parent;
    data_801c55d4_slot04_06[obj->state_0](obj);
}
