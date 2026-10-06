/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80076c74_slot00(Object *o) {
    o->field_06++;
    o->field_0b ^= 1;
    /* func_801380f0 takes no parameter. The original passes the object here
       all the same, and the cast says so. A call through another function
       type than the definition's is not defined in portable C: a port calls
       func_801380f0() without the argument. */
    ((void (*)(Object *))func_801380f0)(o);
    o->field_46 = 2;
    ((Slot00Obj *)o)->field_b3++;
    func_80138070(o, o->field_ad);
}
