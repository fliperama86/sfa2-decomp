/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void func_80078458_slot00(Object *obj, int index);
extern void func_80078b50_slot00(Object *obj);
extern void func_80078b94_slot00(Object *obj);
extern void func_80078d78_slot00(Object *obj);

/* What it does (inferred, not an original name): a handler of an object's
   state that either runs three of the module's update steps and then the
   shared advance_sequence_checked, or, when it must not, counts the object's state_2 up
   by one and calls func_80078458_slot00 with 8. The first arm is taken when
   game_state.field_4e is not 0 and the byte at offset 0x3a of the object that
   field_3c points at is 0. */
void func_80078628_slot00(Object *obj) {
    Slot00Obj *target = (Slot00Obj *)obj->field_3c;

    /* The count and the call of func_80078458_slot00 stand in two arms, once for each test that leads to them. With one arm for both (`if (game_state.field_4e != 0 && target->field_3a == 0) { ... } else { ... }`) this function differs from the original in 5 instruction slots. */
    if (game_state.field_4e == 0) {
        obj->state_2 = obj->state_2 + 1;
        func_80078458_slot00(obj, 8);
    } else if (target->field_3a == 0) {
        func_80078b50_slot00(obj);
        func_80078b94_slot00(obj);
        func_80078d78_slot00(obj);
        advance_sequence_checked(obj);
    } else {
        obj->state_2 = obj->state_2 + 1;
        func_80078458_slot00(obj, 8);
    }
}
