/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800205b4_slot28(Object *obj);
void func_80020608_slot28(Object *o);

/* The call of func_80131094 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_800205b4_slot28(Object *obj) {
    func_80020608_slot28(obj);
    if (obj->pos_y >= 0xd0) {
        obj->field_04++;
    }
    ((void (*)(void))func_80131094)();
}
