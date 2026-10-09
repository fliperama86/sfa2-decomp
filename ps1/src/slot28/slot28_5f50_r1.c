/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015ffc_slot28(Object *obj, SequenceStep *step);
void func_8001601c_slot28(SequenceStep *step);

void func_80015f50_slot28(Object *obj) {
    int t = obj->field_38 - 1;
    int *s;
    obj->field_38 = t;
    if ((s16)t == 0) {
        s = (int *)obj->sequence;
        s += 2;
        if ((s16)obj->field_3a < 0) {
            func_80015ffc_slot28(obj, (SequenceStep *)*s);
            func_8001601c_slot28((SequenceStep *)*s);
        } else {
            func_80015ffc_slot28(obj, (SequenceStep *)s);
            func_8001601c_slot28((SequenceStep *)s);
        }
    }
}
