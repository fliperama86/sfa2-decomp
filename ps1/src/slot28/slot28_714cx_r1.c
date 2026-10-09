/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800306cc_slot28[])(Object *);

/* The call of func_80131094 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_8001714c_slot28(Object *obj) {
    data_800306cc_slot28[obj->field_05](obj);
    ((void (*)(void))func_80131094)();
    obj->field_01 = 1;
}
